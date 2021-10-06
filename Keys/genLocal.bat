
del *.pem

REM define your openssl cnf file - update if needed
set OPENSSL_CONF=C:\git\openssl-3.0.0\openssl-3.0.0\apps\openssl.cnf

REM Generate RootCA.pem, RootCA.key & RootCA.crt:
openssl req -x509 -nodes -new -sha256 -days 1024 -newkey rsa:2048 -keyout RootCA.key -out RootCA.pem -subj "/C=IL/CN=Example-Root-CA"
openssl x509 -outform pem -in RootCA.pem -out RootCA.crt

REM Generate localhost.key, localhost.csr, and localhost.crt:
openssl req -new -nodes -newkey rsa:2048 -keyout localhost.key -out localhost.csr -subj "/C=IL/ST=Israel/L=Rehovot/O=Example-Certificates/CN=localhost.local"
openssl x509 -req -sha256 -days 1024 -in localhost.csr -CA RootCA.pem -CAkey RootCA.key -CAcreateserial -extfile domains.ext -out localhost.crt