#!/bin/bash

SCRIPTSRC=`readlink -f "$0" || echo "$0"`
RUN_PATH=`dirname "${SCRIPTSRC}" || echo .`

install_prefix=${RUN_PATH}/..

function install_desktop_entry()
{
    local installation_prefix="$1"
    local desktop_entries_dir="$2"

    local desktop_entry="\
[Desktop Entry]\n\
Name=Finalverse\n\
Comment=Finalverse persistent virtual world client\n\
Exec=\"${installation_prefix}/finalverse\" %u\n\
Icon=${installation_prefix}/finalverse_icon.png\n\
Terminal=false\n\
Type=Application\n\
Categories=Application;Network;\n\
StartupNotify=true\n\
MimeType=x-scheme-handler/finalverse;\n\
X-Desktop-File-Install-Version=3.0"

    echo " - Installing menu entries in ${desktop_entries_dir}"
    mkdir -vp "${desktop_entries_dir}"
    printf '%b\n' "$desktop_entry" > "${desktop_entries_dir}/finalverse-viewer.desktop" || return 1
}

if [ "${1:-}" = "--output-dir" ] && [ "$#" -eq 2 ]; then
    install_desktop_entry "$install_prefix" "$2"
elif [ "$UID" == "0" ]; then
    # system-wide
    install_desktop_entry "$install_prefix" /usr/local/share/applications
else
    # user-specific
    install_desktop_entry "$install_prefix" "$HOME/.local/share/applications"
fi
