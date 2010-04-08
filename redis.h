#ifndef REDIS_H
#define REDIS_H

#include <string>
#include <list>
#include <map>
#include <vector>
#include "redisCommand.h"
#include "redisResponse.h"
#include "redisSortParams.h"

namespace redis {
class Client {

	typedef Response (Client::*ResponseReader)();

public:
	Client();

	static const int ERROR = -1;
	static const int STRING = 1;
	static const int LIST = 2;
	static const int SET = 3;
	static const int ZSET = 4;
	static const int HASH = 5;

	bool connect(std::string host, short port);
	
	Response auth(Buffer key);
	Response select(int index);
	Response keys(Buffer pattern);
	Response dbsize(int index);
	Response lastsave();
	Response flushdb();
	Response flushall();
	Response save();
	Response bgsave();
	Response bgrewriteaof();
	Response move(Buffer key, int index);
	Response sort(Buffer key);
	Response sort(Buffer key, SortParams params);
	Response type(Buffer key);
	Response append(Buffer key, Buffer padding);
	Response substr(Buffer key, int start, int end);
	Response config(Buffer key, Buffer field);
	Response config(Buffer key, Buffer field, Buffer val);

	Response get(Buffer key);
	Response set(Buffer key, Buffer val);
	Response getset(Buffer key, Buffer val);
	Response incr(Buffer key, int val = 1);
	Response decr(Buffer key, int val = 1);
	Response rename(Buffer src, Buffer dst);
	Response renameNx(Buffer src, Buffer dst);
	Response randomKey();
	Response ttl(Buffer key);
	Response ping();
	Response setNx(Buffer src, Buffer dst);
	Response exists(Buffer key);
	Response del(Buffer key);
	Response del(RedisList key);
	Response mget(RedisList keys);
	Response expire(Buffer key, long ttl);
	Response expireAt(Buffer key, long timestamp);
	Response mset(RedisList keys, RedisList vals);
	Response msetnx(RedisList keys, RedisList vals);
	Response info();

	Response lpush(Buffer key, Buffer val);
	Response rpush(Buffer key, Buffer val);
	Response rpoplpush(Buffer src, Buffer dst);
	Response llen(Buffer key);
	Response lpop(Buffer key);
	Response rpop(Buffer key);
	Response blpop(RedisList keys, int timeout);
	Response brpop(RedisList keys, int timeout);
	Response ltrim(Buffer key, int start, int end);
	Response lindex(Buffer key, int pos);
	Response lrem(Buffer key, int count, Buffer val);
	Response lset(Buffer key, int pos, Buffer val);
	Response lrange(Buffer key, int start, int end);

	Response sadd(Buffer key, Buffer val);
	Response srem(Buffer key, Buffer val);
	Response spop(Buffer key);
	Response scard(Buffer key);
	Response sismember(Buffer key, Buffer val);
	Response srandmember(Buffer key);
	Response smove(Buffer src, Buffer dst, Buffer member);
	Response sinter(RedisList keys);
	Response sunion(RedisList keys);
	Response sdiff(RedisList keys);
	Response sinterstore(RedisList keys);
	Response sunionstore(RedisList keys);
	Response sdiffstore(RedisList keys);

	Response zadd(Buffer key, double score, Buffer member);
	Response zrem(Buffer key, Buffer member);
	Response zincrby(Buffer key, double score, Buffer member);
	Response zscore(Buffer key, Buffer member);
	Response zrank(Buffer key, Buffer member);
	Response zrevrank(Buffer key, Buffer member);
	Response zrange(Buffer key, long start, long end, bool withscores = false);
	Response zrevrange(Buffer key, long start, long end, bool withscores = false);
	Response zcard(Buffer key);
	Response zcount(Buffer key, long start, long end);
	Response zremrangebyrank(Buffer key, long min, long max);
	Response zremrangebyscore(Buffer key, long min, long max);
	Response zrangebyscore(Buffer key, long min, long max, bool withscores = false);
	Response zrangebyscore(Buffer key, long min, long max, long start, long end, bool withscores = false);

	Response zunion(Buffer key, RedisList keys);
	Response zunion(Buffer key, RedisList keys, std::string aggregate);
	Response zunion(Buffer key, RedisList keys, std::vector<double> weights);
	Response zunion(Buffer key, RedisList keys, std::vector<double> weights, std::string aggregate);

	Response zinter(Buffer key, RedisList keys);
	Response zinter(Buffer key, RedisList keys, std::string aggregate);
	Response zinter(Buffer key, RedisList keys, std::vector<double> weights);
	Response zinter(Buffer key, RedisList keys, std::vector<double> weights, std::string aggregate);

	Response hset(Buffer key, Buffer field, Buffer val);
	Response hget(Buffer key, Buffer field);
	Response hdel(Buffer key, Buffer field);
	Response hexists(Buffer key, Buffer field);
	Response hlen(Buffer key);
	Response hkeys(Buffer key);
	Response hvals(Buffer key);
	Response hgetall(Buffer key);
	Response hincrby(Buffer key, Buffer field, double d);

	bool multi();
	bool pipeline();
	void discard();
	std::vector<Response> exec();

private:
	void run(Command &c);
	Response run(Command &c, ResponseReader fun);

	Response generic_key_int_return_int(std::string keyword, Buffer key, int val, bool addBy = false);
	Response generic_push(std::string keyword, Buffer key, Buffer val);
	Response generic_pop(std::string keyword, Buffer key);
	Response generic_list_item_action(std::string keyword, Buffer key, int n, Buffer val, ResponseReader fun);
	Response generic_set_key_value(std::string keyword, Buffer key, Buffer val);
	Response generic_multi_parameter(std::string keyword, RedisList &keys, ResponseReader fun);
	Response generic_zrank(std::string keyword, Buffer key, Buffer member);
	Response generic_zrange(std::string keyword, Buffer key, long start, long end, bool withscores);
	Response generic_z_start_end_int(std::string keyword, Buffer key, long start, long end);
	Response generic_card(std::string keyword, Buffer key);
	Response generic_z_set_operation(std::string keyword, Buffer key, RedisList keys,
		std::vector<double> weights, std::string aggregate);
	Response generic_mset(std::string keyword, RedisList keys, RedisList vals, ResponseReader fun);
	Response generic_h_simple_list(std::string keyword, Buffer key);
	Response generic_blocking_pop(std::string keyword, RedisList keys, int timeout);

	
	Response read_string();
	Response read_integer();
	Response read_double();
	Response read_integer_as_bool();
	Response read_status_code();
	Response read_single_line();
	Response read_multi_bulk();
	Response read_queued();
	Response read_info_reply();
	Response read_type_reply();

	std::string getline();
	int m_fd;

	// MULTI/EXEC
	bool m_multi;
	std::vector<ResponseReader> m_readers;

	// pipeline
	bool m_pipeline;
	Buffer m_cmd;

};
}


#endif /* REDIS_H */
