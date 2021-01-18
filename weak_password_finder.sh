#!/bin/bash

echo "Please enter a password to see if it is weak: "


read user_passwd

regexNumber="[0-9]"
regexSymbol="[@#$%&*+-=]"
pass=true

if [[ ${#user_passwd} -ge 8 ]]; then
    :
else
    pass=false
    echo "Password criteria: Password should be at least 8 characters long"
fi

if [[ $user_passwd =~ $regexNumber ]]; then
    :
else
    pass=false
    echo "Password criteria: Password should contain a number"
fi

if [[ $user_passwd =~ $regexSymbol ]]; then
    :
else
    pass=false
    echo "Password criteria: Password should contain a symbol @, #, $, %, &, *, +, -, ="
fi

if [[ "$pass" = true ]]; then
	echo "The password is strong."
fi
