sysctl kernel.yama.ptrace_scope=0
cd /tmp
mkdir /run/postgresql
chown postgres:postgres $_
mount --bind /home/shared/devel/postgres/555 /srv/pgsql/555
sudo -u postgres pg_ctl -D /srv/pgsql/555 -l logfile start
