#!/bin/bash

# Send a world URL to Finalverse; the inherited filename is a packaging interface.
#

URL="$1"

if [ -z "$URL" ]; then
    echo Usage: $0 secondlife://...
    exit
fi

RUN_PATH=`dirname "$0" || echo .`
cd "${RUN_PATH}/.."

exec ./finalverse --url "${URL}"
