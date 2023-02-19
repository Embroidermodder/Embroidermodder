#!/usr/bin/env python3

"""
 *  Daily Build Github Action
 *  This file is part of Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
"""

import os
import json
import time

stub = "embroidermodder-2.0.0-alpha-"
linux_fname = stub + "linux.zip"
macos_fname = stub + "macos.zip"
windows_fname = stub + "windows.zip"

def download_gh(url, fname):
    TOKEN = os.getenv("GH_TOKEN")
    curl_cmd = """curl \
        -H "Accept: application/vnd.github+json" \
        -H "Authorization: Bearer %s"\
        -H "X-GitHub-Api-Version: 2022-11-28" """ % (TOKEN)

    with open("command.sh", "w") as f:
        f.write(curl_cmd + url + " > " + fname + "\n")
        f.write("echo \"Success\"")
    time.sleep(1)

    os.system("sh command.sh")
    time.sleep(1)

def load_gh_json(url, fname):
    download_gh(url, fname)
    d = {}
    with open(fname, "r") as f:
        d = json.load(f)
    print(json.dumps(d, indent=4))
    print("")
    return d

def upload_gh_release_asset(asset_id, fname, label):
    TOKEN = os.getenv("GH_TOKEN")
    os.system("""curl \
        -X PATCH \
        -H "Accept: application/vnd.github+json" \
        -H "Authorization: Bearer %s"\
        -H "X-GitHub-Api-Version: 2022-11-28" \
        https://api.github.com/repos/Embroidermodder/Embroidermodder/releases/assets/%d \
        -d '{"name":"%s","label":"%s"}'
    """ % (TOKEN, asset_id, fname, label))


d = load_gh_json("https://api.github.com/repos/Embroidermodder/Embroidermodder/actions/runs", "workflow_history.json")

most_recent = {"run_number": 0}
for run in d["workflow_runs"]:
    if run["name"] == "build":
        if run["run_number"] > most_recent["run_number"]:
            most_recent = run

artifacts = load_gh_json(most_recent["artifacts_url"], "artifacts.json")

suite_id = most_recent["check_suite_id"]

for a in artifacts["artifacts"]:
    print("Downloading: ", a["id"])
    download_gh(a["archive_download_url"], a["name"] + ".zip")

releases = load_gh_json("https://api.github.com/repos/Embroidermodder/Embroidermodder/releases", "releases.json")

release = releases[0]

for r in releases:
    if r["tag_name"] == "alpha1":
        release = r

url = "https://api.github.com/repos/Embroidermodder/Embroidermodder/releases/%d/assets" % (release["id"])

assets = load_gh_json(url, "assets.json")

linux_id = 0
macos_id = 0
windows_id = 0
for asset in assets:
    if asset["name"] == linux_fname:
        linux_id = asset["id"]
    if asset["name"] == macos_fname:
        macos_id = asset["id"]
    if asset["name"] == windows_fname:
        windows_id = asset["id"]

upload_gh_release_asset(linux_id, linux_fname, "GNU/Linux")
upload_gh_release_asset(macos_id, macos_fname, "Mac OS")
upload_gh_release_asset(windows_id, windows_fname, "Windows")
