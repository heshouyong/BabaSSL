# Overview

BabaSSL is a modern cryptographic and secure protocol library developed by the amazing people in Alibaba Digital Economy.

# Feature

BabaSSL provides the following major features:
  * Support [RFC 8998](https://tools.ietf.org/html/rfc8998), Chinese SM cipher suites in TLS 1.3 protocol
  * Support NTLS(formal GM double cert) handshake processing, according to GB/T 38636-2020 TLCP
  * QUIC API support
  * Support delegated credentials, according to [draft-ietf-tls-subcerts-10](https://www.ietf.org/archive/id/draft-ietf-tls-subcerts-10.txt)
  * ...

# Compile
 * for linux_x86_64  

git clone https://github.com/heshouyong/BabaSSL.git  

./Configure --prefix=$(pwd)/install no-asm shared linux-x86_64  

make && make install  

 * for linux arm   

./Configure --prefix=$(pwd)/install --cross-compile-prefix=arm-linux-gnueabihf- no-asm shared linux-armv4   

 * for linux aarch64  

./Configure --prefix=$(pwd)/install --cross-compile-prefix=aarch64-linux-gnu- no-asm shared linux-aarch64  

# Reporting Security Bugs

We use the following website for security bugs reporting:

  * https://security.alipay.com/

Note: for other non-security related bugs, please use Github Issues instead.
