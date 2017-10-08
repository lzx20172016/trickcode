

#启动cgi程序
sudo /usr/local/nginx/sbin/nginx -s reload
kill -9 `ps aux | grep "demo_cgi" | grep -v grep | awk '{print $2}'`
kill -9 `ps aux | grep "echo_cgi" | grep -v grep | awk '{print $2}'`
kill -9 `ps aux | grep "upload_cgi" | grep -v grep | awk '{print $2}'`
kill -9 `ps aux | grep "data_cgi" | grep -v grep | awk '{print $2}'`
kill -9 `ps aux | grep "login_cgi" | grep -v grep | awk '{print $2}'`
kill -9 `ps aux | grep "reg_cgi" | grep -v grep | awk '{print $2}'`
spawn-fcgi -a 127.0.0.1 -p 8082 -f ./demo_cgi
spawn-fcgi -a 127.0.0.1 -p 8083 -f ./echo_cgi
spawn-fcgi -a 127.0.0.1 -p 8084 -f ./upload_cgi
spawn-fcgi -a 127.0.0.1 -p 8085 -f ./data_cgi
spawn-fcgi -a 127.0.0.1 -p 8086 -f ./login_cgi
spawn-fcgi -a 127.0.0.1 -p 8087 -f ./reg_cgi
