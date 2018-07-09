#!/bin/bash

echo -e "This is a notification message" | \
mailx -v \
-s "Sent from my host" \
-S smtp-use-starttls \
-S ssl-verify=ignore \
-S smtp-auth=login \
-S smtp=smtp://smtp.gmail.com:587 \
-S from="jaredmoxa@gmail.com" \
-S smtp-auth-user=ur_account@gmail.com \
-S smtp-auth-password=ur_password \
-S ssl-verify=ignore \
-S nss-config-dir=/root/.certs \
target-account@xxx.xxx.com
