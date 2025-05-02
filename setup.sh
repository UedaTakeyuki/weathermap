sudo ln -s $(pwd)/weathermap.rsyslog.conf /etc/rsyslog.d/weathermap.rsyslog.conf
# sudo touch /var/log/atelierueda/weathermap.log
sudo systemctl restart rsyslog.service
