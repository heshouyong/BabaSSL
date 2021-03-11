/*
 * Copyright 2013-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * Simple SM4 CCM test program.
 * self test but uses the application level EVP APIs.
 */
#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

/* SM4-CCM test data from NIST public test vectors */

static const unsigned char ccm_key[] = {
    0xca, 0x44, 0xef, 0x8d, 0xf3, 0x25, 0xab, 0xb3, 
    0x8d, 0xac, 0x37, 0x43, 0xdd, 0x32, 0x43, 0xdf
};

static const unsigned char ccm_nonce[] = {
    0x72, 0x43, 0x52, 0x3c, 0x65, 0x17, 0x8b, 0x96, 0x68, 0x37, 0xa3, 0x6f
};

static const unsigned char ccm_adata[] = {
    0x6e, 0x80, 0xdd, 0x7f, 0x1b, 0xad, 0xf3, 0xa1, 0xc9, 0xab, 0x25, 0xc7,
    0x5f, 0x10, 0xbd, 0xe7, 0x8c, 0x23, 0xfa, 0x0e, 0xb8, 0xf9, 0xaa, 0xa5,
    0x3a, 0xde, 0xfb, 0xf4, 0xcb, 0xf7, 0x8f, 0xe4
};
//plaintext
static const unsigned char ccm_pt[] = {
    0x03, 0x80, 0x14, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
    0xa9, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9
};
//ciphertext
static const unsigned char ccm_ct[] = {
    0xfb, 0x78, 0x40, 0x15, 0x24, 0xca, 0x9c, 0x2d, 0x68, 0x3b, 0xc4, 0xe9,
    0x5d, 0xdc, 0x71, 0xb8, 0x28, 0x07, 0x77, 0x81, 0xaa, 0x8e, 0x3f
};

static const unsigned char ccm_tag[] = {
    0xe8, 0x1c, 0xe4, 0xde, 0x21, 0x38, 0x76, 0x49, 0x19, 0x59, 0xee, 0x87,
    0x63, 0xe2, 0x21, 0x55
};

void sm4_ccm_encrypt(void)
{
    EVP_CIPHER_CTX *ctx;
    int outlen, tmplen;
    unsigned char outbuf[1024];
    printf("SM4 CCM Encrypt:\n");
    printf("Plaintext:\n");
    BIO_dump_fp(stdout, ccm_pt, sizeof(ccm_pt));
    ctx = EVP_CIPHER_CTX_new();
    /* Set cipher type and mode */
    EVP_EncryptInit_ex(ctx, EVP_sm4_ccm(), NULL, NULL, NULL);
    /* Set nonce length if default 96 bits is not appropriate */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, sizeof(ccm_nonce),
                        NULL);
    /* Set tag length */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG, sizeof(ccm_tag), NULL);
    /* Initialise key and IV */
    EVP_EncryptInit_ex(ctx, NULL, NULL, ccm_key, ccm_nonce);
    /* Encrypt plaintext: can only be called once */
    EVP_EncryptUpdate(ctx, outbuf, &outlen, ccm_pt, sizeof(ccm_pt));
    /* Output encrypted block */
    printf("Ciphertext:\n");
    BIO_dump_fp(stdout, outbuf, outlen);
    /* Finalise: note get no output for CCM */
    EVP_EncryptFinal_ex(ctx, outbuf, &outlen);
    /* Get tag */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_GET_TAG, 16, outbuf);
    /* Output tag */
    printf("Tag:\n");
    BIO_dump_fp(stdout, outbuf, 16);
    EVP_CIPHER_CTX_free(ctx);
}

void sm4_ccm_decrypt(void)
{
    EVP_CIPHER_CTX *ctx;
    int outlen, tmplen, rv;
    unsigned char outbuf[1024];
    printf("SM4 CCM Derypt:\n");
    printf("Ciphertext:\n");
    BIO_dump_fp(stdout, ccm_ct, sizeof(ccm_ct));
    ctx = EVP_CIPHER_CTX_new();
    /* Select cipher */
    EVP_DecryptInit_ex(ctx, EVP_sm4_ccm(), NULL, NULL, NULL);
    /* Set nonce length, omit for 96 bits */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, sizeof(ccm_nonce),
                        NULL);
    /* Set expected tag value */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG,
                        sizeof(ccm_tag), (void *)ccm_tag);
    /* Specify key and IV */
    EVP_DecryptInit_ex(ctx, NULL, NULL, ccm_key, ccm_nonce);
    /* Decrypt plaintext, verify tag: can only be called once */
    rv = EVP_DecryptUpdate(ctx, outbuf, &outlen, ccm_ct, sizeof(ccm_ct));
    /* Output decrypted block: if tag verify failed we get nothing */
    if (rv > 0) {
        printf("Plaintext:\n");
        BIO_dump_fp(stdout, outbuf, outlen);
    } else
        printf("Plaintext not available: tag verify failed.\n");
    EVP_CIPHER_CTX_free(ctx);
}

int main(int argc, char **argv)
{
    sm4_ccm_encrypt();
    sm4_ccm_decrypt();
}
