youtube-dl -f bestvideo+bestaudio --output '%(uploader)s/%(upload_date)s__%(id)s :: %(title)s.%(ext)s' --write-description --write-thumbnail --write-info-json --all-subs $1
