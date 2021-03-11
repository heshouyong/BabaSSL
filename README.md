# Overview

BabaSSL is a modern cryptographic and secure protocol library developed by the amazing people in Alibaba Digital Economy.

# Feature

BabaSSL provides the following major features:
  * Chinese SM cipher suites in TLS 1.3 protocol
  * QUIC API support
  * ...

# Compile
 * for linux_x86_64
git clone https://github.com/heshouyong/BabaSSL.git
./Configure --prefix=$(pwd)/install no-asm shared linux-x86_64
make && make install

# Reporting Security Bugs

We use the following website for security bugs reporting:

  * https://security.alipay.com/

Note: for other non-security related bugs, please use Github Issues instead.
