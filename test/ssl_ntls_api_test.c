#include <stdio.h>
#include <string.h>

#include <openssl/ssl.h>
#include "testutil.h"
#include "../ssl/ssl_local.h"

static const char *sign_cert_file;
static const char *sign_key_file;
static const char *enc_cert_file;
static const char *enc_key_file;

static int test_ntls_ctx_set_cert_pkey_file_api(void)
{
    int           ret = 1;
#ifndef OPENSSL_NO_NTLS
    SSL_CTX      *ctx = NULL;

    ret = 0;
    ctx = SSL_CTX_new(NTLS_method());
    if (!TEST_true(ctx != NULL))
        goto err;

    SSL_CTX_enable_ntls(ctx);
    if (!TEST_true(ctx->enable_ntls == 1))
        goto err;
    SSL_CTX_disable_ntls(ctx);
    if (!TEST_true(ctx->enable_ntls == 0))
        goto err;

    if (!TEST_int_eq(SSL_CTX_use_sign_certificate_file(ctx,
                                                       sign_cert_file, SSL_FILETYPE_PEM), 1))
        goto err;

    if (!TEST_true(ctx->cert->pkeys[SSL_PKEY_SM2_SIGN].x509 != NULL))
        goto err;


    if (!TEST_int_eq(SSL_CTX_use_sign_PrivateKey_file(ctx,
                                                      sign_key_file, SSL_FILETYPE_PEM), 1))
        goto err;

    if (!TEST_true(ctx->cert->pkeys[SSL_PKEY_SM2_SIGN].privatekey != NULL))
        goto err;


    if (!TEST_int_eq(SSL_CTX_use_enc_certificate_file(ctx,
                                                      enc_cert_file, SSL_FILETYPE_PEM), 1))
        goto err;

    if (!TEST_true(ctx->cert->pkeys[SSL_PKEY_SM2_ENC].x509 != NULL))
        goto err;

    if (!TEST_int_eq(SSL_CTX_use_enc_PrivateKey_file(ctx,
                                                     enc_key_file, SSL_FILETYPE_PEM), 1))
        goto err;

    if (!TEST_true(ctx->cert->pkeys[SSL_PKEY_SM2_ENC].privatekey != NULL))
        goto err;

    ret = 1;
err:
    SSL_CTX_free(ctx);
#endif
    return ret;
}

static int test_ntls_ssl_set_cert_pkey_file_api(void)
{
    int           ret = 1;
#ifndef OPENSSL_NO_NTLS
    SSL_CTX      *ctx = NULL;
    SSL          *ssl = NULL;

    ret = 0;
    ctx = SSL_CTX_new(NTLS_method());
    if (!TEST_true(ctx != NULL))
        goto err;

    ssl = SSL_new(ctx);
    if (!TEST_true(ssl != NULL))
        goto err;

    if (!TEST_true(SSL_is_ntls(ssl) == 1))
        goto err;

    SSL_enable_ntls(ssl);
    if (!TEST_true(ssl->enable_ntls == 1))
        goto err;
    SSL_disable_ntls(ssl);
    if (!TEST_true(ssl->enable_ntls == 0))
        goto err;

    if (!TEST_int_eq(SSL_use_sign_certificate_file(ssl,
                                                   sign_cert_file, SSL_FILETYPE_PEM), 1))
        goto err;
    if (!TEST_true(ssl->cert->pkeys[SSL_PKEY_SM2_SIGN].x509 != NULL))
        goto err;


    if (!TEST_int_eq(SSL_use_sign_PrivateKey_file(ssl,
                                                  sign_key_file, SSL_FILETYPE_PEM), 1))
        goto err;
    if (!TEST_true(ssl->cert->pkeys[SSL_PKEY_SM2_SIGN].privatekey != NULL))
        goto err;

    if (!TEST_int_eq(SSL_use_enc_certificate_file(ssl,
                                                  enc_cert_file, SSL_FILETYPE_PEM), 1))
        goto err;
    if (!TEST_true(ssl->cert->pkeys[SSL_PKEY_SM2_ENC].x509 != NULL))
        goto err;


    if (!TEST_int_eq(SSL_use_enc_PrivateKey_file(ssl,
                                                 enc_key_file, SSL_FILETYPE_PEM), 1))
        goto err;
    if (!TEST_true(ssl->cert->pkeys[SSL_PKEY_SM2_ENC].privatekey != NULL))
        goto err;

    ret = 1;
err:
    SSL_CTX_free(ctx);
    SSL_free(ssl);
#endif
    return ret;
}


static int test_ntls_ctx_set_cert_pkey_api(void)
{
    int           ret = 1;
#ifndef OPENSSL_NO_NTLS
    SSL_CTX      *ctx = NULL;
    X509         *sign_cert = NULL;
    EVP_PKEY     *sign_pkey = NULL;
    X509         *enc_cert = NULL;
    EVP_PKEY     *enc_pkey = NULL;
    BIO          *sign_cert_bio = NULL;
    BIO          *sign_pkey_bio = NULL;
    BIO          *enc_cert_bio = NULL;
    BIO          *enc_pkey_bio = NULL;

    ret = 0;
    sign_cert_bio = BIO_new(BIO_s_file());
    enc_cert_bio = BIO_new(BIO_s_file());
    if (!TEST_ptr(sign_cert_bio) || !TEST_ptr(enc_cert_bio))
        goto err;

    if (!TEST_int_eq(BIO_read_filename(sign_cert_bio, sign_cert_file), 1)
        || !TEST_int_eq(BIO_read_filename(enc_cert_bio, enc_cert_file), 1))
        goto err;

    sign_cert = PEM_read_bio_X509(sign_cert_bio, NULL, NULL, NULL);
    enc_cert = PEM_read_bio_X509(enc_cert_bio, NULL, NULL, NULL);
    if (!TEST_ptr(sign_cert) || !TEST_ptr(enc_cert))
        goto err;

    sign_pkey_bio = BIO_new(BIO_s_file());
    enc_pkey_bio = BIO_new(BIO_s_file());
    if (!TEST_ptr(sign_pkey_bio) || !TEST_ptr(enc_pkey_bio))
        goto err;

    if (!TEST_int_eq(BIO_read_filename(sign_pkey_bio, sign_key_file), 1)
        || !TEST_int_eq(BIO_read_filename(enc_pkey_bio, enc_key_file), 1))
        goto err;

    sign_pkey = PEM_read_bio_PrivateKey(sign_pkey_bio, NULL, NULL, NULL);
    enc_pkey = PEM_read_bio_PrivateKey(enc_pkey_bio, NULL, NULL, NULL);
    if (!TEST_ptr(sign_pkey) || !TEST_ptr(enc_pkey))
        goto err;


    ctx = SSL_CTX_new(NTLS_method());
    if (!TEST_true(ctx != NULL))
        goto err;


    if (!TEST_int_eq(SSL_CTX_use_sign_certificate(ctx, sign_cert), 1))
        goto err;

    if (!TEST_true(ctx->cert->pkeys[SSL_PKEY_SM2_SIGN].x509 != NULL))
        goto err;

    if (!TEST_int_eq(SSL_CTX_use_sign_PrivateKey(ctx, sign_pkey), 1))
        goto err;

    if (!TEST_true(ctx->cert->pkeys[SSL_PKEY_SM2_SIGN].privatekey != NULL))
        goto err;


    if (!TEST_int_eq(SSL_CTX_use_enc_certificate(ctx, enc_cert), 1))
        goto err;

    if (!TEST_true(ctx->cert->pkeys[SSL_PKEY_SM2_ENC].x509 != NULL))
        goto err;

    if (!TEST_int_eq(SSL_CTX_use_enc_PrivateKey(ctx, enc_pkey), 1))
        goto err;

    if (!TEST_true(ctx->cert->pkeys[SSL_PKEY_SM2_ENC].privatekey != NULL))
        goto err;

    ret = 1;
err:
    BIO_free(sign_cert_bio);
    BIO_free(enc_cert_bio);
    BIO_free(sign_pkey_bio);
    BIO_free(enc_pkey_bio);
    X509_free(sign_cert);
    X509_free(enc_cert);
    EVP_PKEY_free(sign_pkey);
    EVP_PKEY_free(enc_pkey);
    SSL_CTX_free(ctx);
#endif
    return ret;
}

static int test_ntls_ssl_set_cert_pkey_api(void)
{
    int           ret = 1;
#ifndef OPENSSL_NO_NTLS
    SSL_CTX      *ctx = NULL;
    SSL          *ssl = NULL;
    X509         *sign_cert = NULL;
    EVP_PKEY     *sign_pkey = NULL;
    X509         *enc_cert = NULL;
    EVP_PKEY     *enc_pkey = NULL;
    BIO          *sign_cert_bio = NULL;
    BIO          *sign_pkey_bio = NULL;
    BIO          *enc_cert_bio = NULL;
    BIO          *enc_pkey_bio = NULL;

    ret = 0;
    sign_cert_bio = BIO_new(BIO_s_file());
    enc_cert_bio = BIO_new(BIO_s_file());
    if (!TEST_ptr(sign_cert_bio) || !TEST_ptr(enc_cert_bio))
        goto err;
    if (!TEST_int_eq(BIO_read_filename(sign_cert_bio, sign_cert_file), 1)
        || !TEST_int_eq(BIO_read_filename(enc_cert_bio, enc_cert_file), 1))
        goto err;
    sign_cert = PEM_read_bio_X509(sign_cert_bio, NULL, NULL, NULL);
    enc_cert = PEM_read_bio_X509(enc_cert_bio, NULL, NULL, NULL);
    if (!TEST_ptr(sign_cert) || !TEST_ptr(enc_cert))
        goto err;

    sign_pkey_bio = BIO_new(BIO_s_file());
    enc_pkey_bio = BIO_new(BIO_s_file());
    if (!TEST_ptr(sign_pkey_bio) || !TEST_ptr(enc_pkey_bio))
        goto err;
    if (!TEST_int_eq(BIO_read_filename(sign_pkey_bio, sign_key_file), 1)
        || !TEST_int_eq(BIO_read_filename(enc_pkey_bio, enc_key_file), 1))
        goto err;
    sign_pkey = PEM_read_bio_PrivateKey(sign_pkey_bio, NULL, NULL, NULL);
    enc_pkey = PEM_read_bio_PrivateKey(enc_pkey_bio, NULL, NULL, NULL);
    if (!TEST_ptr(sign_pkey) || !TEST_ptr(enc_pkey))
        goto err;

    ctx = SSL_CTX_new(NTLS_method());
    if (!TEST_true(ctx != NULL))
        goto err;
    ssl = SSL_new(ctx);
    if (!TEST_true(ssl != NULL))
        goto err;

    if (!TEST_int_eq(SSL_use_sign_certificate(ssl, sign_cert), 1))
        goto err;

    if (!TEST_true(ssl->cert->pkeys[SSL_PKEY_SM2_SIGN].x509 != NULL))
        goto err;

    if (!TEST_int_eq(SSL_use_sign_PrivateKey(ssl, sign_pkey), 1))
        goto err;

    if (!TEST_true(ssl->cert->pkeys[SSL_PKEY_SM2_SIGN].privatekey != NULL))
        goto err;

    if (!TEST_int_eq(SSL_use_enc_certificate(ssl, enc_cert), 1))
        goto err;

    if (!TEST_true(ssl->cert->pkeys[SSL_PKEY_SM2_ENC].x509 != NULL))
        goto err;

    if (!TEST_int_eq(SSL_use_enc_PrivateKey(ssl, enc_pkey), 1))
        goto err;

    if (!TEST_true(ssl->cert->pkeys[SSL_PKEY_SM2_ENC].privatekey != NULL))
        goto err;

    ret = 1;
err:
    BIO_free(sign_cert_bio);
    BIO_free(enc_cert_bio);
    BIO_free(sign_pkey_bio);
    BIO_free(enc_pkey_bio);
    X509_free(sign_cert);
    X509_free(enc_cert);
    EVP_PKEY_free(sign_pkey);
    EVP_PKEY_free(enc_pkey);
    SSL_CTX_free(ctx);
    SSL_free(ssl);
#endif
    return ret;
}

static int test_ntls_method_api(void)
{
    int ret = 1;
#ifndef OPENSSL_NO_NTLS
    const SSL_METHOD *meth = NULL;

    ret = 0;
    meth = NTLS_method();
    if (!TEST_true(meth->version == NTLS_VERSION))
        goto err;
    if (!TEST_true(meth->flags == SSL_METHOD_NO_SUITEB))
        goto err;
    if (!TEST_true(meth->mask == SSL_OP_NO_NTLS))
        goto err;

    meth = NTLS_server_method();
    if (!TEST_true(meth->version == NTLS_VERSION))
        goto err;
    if (!TEST_true(meth->flags == SSL_METHOD_NO_SUITEB))
        goto err;
    if (!TEST_true(meth->mask == SSL_OP_NO_NTLS))
        goto err;

    meth = NTLS_client_method();
    if (!TEST_true(meth->version == NTLS_VERSION))
        goto err;
    if (!TEST_true(meth->flags == SSL_METHOD_NO_SUITEB))
        goto err;
    if (!TEST_true(meth->mask == SSL_OP_NO_NTLS))
        goto err;

    ret = 1;
err:
#endif
    return ret;
}

int setup_tests(void)
{
    if (!TEST_ptr(sign_cert_file = test_get_argument(0))
            || !TEST_ptr(sign_key_file = test_get_argument(1))
            || !TEST_ptr(enc_cert_file = test_get_argument(2))
            || !TEST_ptr(enc_key_file = test_get_argument(3))) {
        TEST_note("usage: ssl_ntls_api_test cert.pem|key.pem");
        return 0;
    }
    ADD_TEST(test_ntls_ctx_set_cert_pkey_file_api);
    ADD_TEST(test_ntls_ctx_set_cert_pkey_api);
    ADD_TEST(test_ntls_ssl_set_cert_pkey_file_api);
    ADD_TEST(test_ntls_ssl_set_cert_pkey_api);
    ADD_TEST(test_ntls_method_api);
    return 1;
}