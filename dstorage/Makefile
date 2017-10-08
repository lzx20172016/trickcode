
CC=gcc
CPPFLAGS=-I ./include -I/usr/local/include/hiredis -I./redis_op -I./cjson_test/ -I /usr/include/mysql/
CFLAGS=-Wall
LIBS= -lfdfsclient -lpthread -lfcgi -lhiredis -lmysqlclient

main=./main
test_hiredis=./redis_op/test_hiredis
test_redis_op=./redis_op/test_redis_op
json_test=./cjson_test/json_test
save_one_file=./redis_op/save_one_file
demo_cgi=./demo_cgi
echo_cgi=./echo_cgi
upload_cgi=./upload_cgi
data_cgi=./data_cgi
login_cgi=./login_cgi
reg_cgi=./reg_cgi
target=$(main) $(demo_cgi) $(echo_cgi) $(upload_cgi) $(test_hiredis) $(test_redis_op) $(save_one_file) $(data_cgi) $(json_test) $(login_cgi) $(reg_cgi)

ALL:$(target)

%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)

$(demo_cgi):demo_cgi.o 
	$(CC) $^ -o $@ $(LIBS)
$(main):./main.o ./fdfs_api.o
	$(CC) $^ -o $@ $(LIBS)
$(echo_cgi):echo_cgi.o 
	$(CC) $^ -o $@ $(LIBS)
$(upload_cgi):upload_cgi.o util.o ./redis_op/upload_file.o ./redis_op/redis_op.o ./redis_op/make_log.o ./redis_op/fdfs_api.o ./redis_op/cJSON.o
	$(CC) $^ -o $@ $(LIBS)
$(test_hiredis):./redis_op/test_hiredis_api.o
	$(CC) $^ -o $@ $(LIBS)
$(test_redis_op):./redis_op/test_redis_op.o ./redis_op/redis_op.o ./redis_op/make_log.o
	$(CC) $^ -o $@ $(LIBS)
$(save_one_file):./redis_op/save_one_file.o ./redis_op/redis_op.o ./redis_op/fdfs_api.o ./redis_op/make_log.o
	$(CC) $^ -o $@ $(LIBS)
$(data_cgi):data_cgi.o util.o ./redis_op/make_log.o ./redis_op/cJSON.o ./redis_op/upload_file.o ./redis_op/redis_op.o ./redis_op/fdfs_api.o
	$(CC) $^ -o $@ $(LIBS)
$(login_cgi):login_cgi.o util.o ./redis_op/make_log.o ./redis_op/cJSON.o ./redis_op/upload_file.o ./redis_op/redis_op.o ./redis_op/fdfs_api.o ./dao_mysql.o
	$(CC) $^ -o $@ $(LIBS)
$(reg_cgi):reg_cgi.o util.o ./redis_op/make_log.o ./redis_op/cJSON.o ./redis_op/upload_file.o ./redis_op/redis_op.o ./redis_op/fdfs_api.o ./dao_mysql.o 
	$(CC) $^ -o $@ $(LIBS)

$(json_test):./cjson_test/cJSON_TEST.o ./cjson_test/cJSON.o
	$(CC) $^ -o $@ $(LIBS)

clean:
	-rm -rf ./*.o $(target) ./test/*.o

distclean:
	-rm -rf ./*.o $(target) ./test/*.o
