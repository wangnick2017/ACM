/*
	database接口
	D为数据类型，K为索引类型
*/

template<class D, class K>
class DB{

	void init(char *filename, int key_num){
		/*
			初始化数据库，如果没有对应文件则新建
			key_num为索引个数
		*/
	}
	
	void insert(D data, K *key) {
		/*
			插入一个数据，数据为data，key为包含key_num个索引的数组，其中key[0]为主key（保证唯一）
		*/
	}

	void modify(K key, D n_data, K *n_key){
		/*
			修改，key为主索引，n_data和n_key为修改后data和key
		*/
	}

	pair<D*, bool> find_main(K key) {
		/*
			查询主索引为key的数据
			未找到则返回(NULL, false)
			成功为(指向目标data的指针, true)
		*/
	}

	pair<D*, int> find_sub(K key, int pos) {
		/*
			查询满足索引数组中下标为pos处值为key的所有data
			返回数组和满足条件的个数
		*/
	}

	void del(K key) {
		/*
			删除主索引为key的数据
		*/	
	}

}