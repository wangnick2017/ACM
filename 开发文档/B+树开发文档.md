

# B+树开发文档 #

## 一.实现思路##

## 1.1实现思路 ##

B+树的实现思路大同小异，本次实现主要是使用了WIkipedia的实现方法。

> ===Insertion===
>
> * Perform a search to determine what bucket the new record should go into.
> * If the bucket is not full (at most  b - 1 entries after the insertion), add the record.
> * Otherwise, ''before'' inserting the new record
>   ** split the bucket.
>   *** original node has (L+1)/2 items
>   *** new node has (L+1)/2 items
>   ** Move (L+1)/2-th key to the parent, and insert the new node to the parent.
>   ** Repeat until a parent is found that need not split.
> * If the root splits, treat it as if it has an empty parent and split as outline above.
>   B-trees grow at the root and not at the leaves.

> ​	===Deletion===
>
> * Start at root, find leaf ''L'' where entry belongs.
> * Remove the entry.
>   ** If ''L'' is at least half-full, done
>   ** If ''L'' has fewer entries than it should,
>   *** If sibling (adjacent node with same parent as ''L'') is more than half-full, re-distribute, borrowing an entry from it.
>   *** Otherwise, sibling is exactly half-full, so we can merge ''L'' and sibling.
> * If merge occurred, must delete entry (pointing to ''L'' or sibling) from parent of ''L''.
> * Merge could propagate to root, decreasing height.

>===Bulk-loading===
>Given a collection of data records, we want to create a B+ tree index on some key field. One approach is to insert each record into an empty tree. However, it is quite expensive, because each entry requires us to start from the root and go down to the appropriate leaf page. An efficient alternative is to use bulk-loading.
>
>* The first step is to sort the data entries according to a search key in ascending order.
>* We allocate an empty page to serve as the root, and insert a pointer to the first page of entries into it.
>* When the root is full, we split the root, and create a new root page.
>* Keep inserting entries to the right most index page just above the leaf level, until all entries are indexed.
>
>Note :
>* when the right-most index page above the leaf level fills up, it is split;
>* this action may, in turn, cause a split of the right-most index page on step closer to the root;
>* splits only occur on the right-most path from the root to the leaf level.

当然我的实现方式有所不同，我在实现过程中没用像书中介绍的那样间隔存储关键字和记录数据（如下图）：

---

![img](/home/kstarxin/Documents/90938166.jpg)

​											一棵五阶B+树

---

我将磁盘地址记录和关键字合并为一个struct，作为树上的记录。



## 二.实现方式 ##

### 2.1 B+树的存储实现 ###

#### 2.1.1 文件结构 ####

​	本次开发的B+树在运行时共会生成四个文件`.ksxidx`文件，`.ksxdb`文件，`.dbmgr` 文件以及`.idxmgr`文件，其分别对应的是索引文件，数据文件，索引管理文件以及数据管理文件。

	##### 	2.1.1.1索引文件 #####

​		存储B+树的节点以及B+树本身的信息，严格4KB读写。

	##### 	2.1.1.2 数据文件

​		单纯存储数据，没有特殊操作

	##### 	2.1.1.3 管理文件

​		管理索引文件以及数据文件上的可用空间，防止文件大小增长过快

---

#### 2.1.2 抽象数据类型 ####

	##### 2.1.2.1 B+树自身文件信息	

---

```C++
private:
    char idxFileName[MAX_FILENAME_LEN]; //文件名
    char idxFileMgr[MAX_FILENAME_LEN];
    char dbFileName[MAX_FILENAME_LEN];
    char dbFileMgr[MAX_FILENAME_LEN];
    OFFSET_TYPE dataSize;//数据库大小
    OFFSET_TYPE rootOffset;//根节点位置、
	BPTNode* currentNode;//常驻内存的根节点
```

---



##### 2.1.2.2 B+树节点信息#####

---

```c++
    struct BPTNode{
        char nodeType = DELETED; //节点类型
        short sz = 0;//节点中数据个数
        OFFSET_TYPE nodeOffset = INVALID_OFFSET; //节点在文件中的位置
        OFFSET_TYPE nextNode = INVALID_OFFSET; //同一层后继节点的位置
        OFFSET_TYPE prevNode = INVALID_OFFSET;//同一层前驱节点的位置
        treeData data[MAX_BLOCK_SIZE];//节点数据
    };

```

---



##### 2.1.2.3节点数据信息 #####

---

```c++
    struct treeData{
        Key k = Key(); //关键字
        OFFSET_TYPE data = INVALID_OFFSET; //文件位置指针
    };
```

---



#### 2.1.3 读写方式 ###

由于高频次的开关文件以及外存访问成本非常高昂，因此必须很好的优化读写才能使B+树的运行效率变高。在本次B+树实现中主要对索引节点的读取做了4K读写（通过计算使得节点大小接近4KB，提高文件利用效率）。在同样的提条件和同样的数据规模下，进过4K读写优化的B+树可以节省大约一半的时间。

将接近4KB的数据写入一个大小为4KB的`rwBuffer`数组中，然后再写入磁盘。

读写由若干函数完成，这里简单介绍`writeNode()`以及`writeIdx()`函数。

`writeNode()`函数：

```C++
    bool writeNode(BPTNode *p, OFFSET_TYPE offset = 0){
        if(fidx.fail()) fidx.close();
        if(!fidx.is_open()) fidx.open(idxFileName, IOB);
        //确保文件正常开启
        if(offset == 0){
            fidx.seekg(0, std::ios_base::end);
            offset = fidx.tellg();
        }
        p->nodeOffset = offset;
        if(fidx.fail()){
            fidx.close();
            return 0;
        }
        fidx.seekp(offset);
        memcpy(rwBuffer, p, sizeof(BPTNode)); //将待写入数据拷贝到缓冲区
        fidx.write(rwBuffer, sizeof(char) * MAX_BUFFER_SIZE);：
        fidx.flush();
        return 1;
    }
```

---



`writeIdx()`函数：

```C++
    bool writeIdx(){
        if(!fidx.is_open()) fidx.open(idxFileName, IOB);
        fidx.seekp(0);
        OFFSET_TYPE pos = 0;
        memcpy(&(rwBuffer[pos]), idxFileName, sizeof(char) * MAX_FILENAME_LEN);
        pos += sizeof(char) * MAX_FILENAME_LEN;
        memcpy(&(rwBuffer[pos]), dbFileName, sizeof(char) * MAX_FILENAME_LEN);
        pos += sizeof(char) * MAX_FILENAME_LEN;
        memcpy(&(rwBuffer[pos]), &dataSize, sizeof(OFFSET_TYPE));
        pos += sizeof(OFFSET_TYPE);
        memcpy(&(rwBuffer[pos]), &rootOffset, sizeof(OFFSET_TYPE));
        //类似的操作，将数据拷贝到缓冲区，再进行写入
        fidx.write(rwBuffer, MAX_BUFFER_SIZE);
        fidx.flush();
        return 1;
    }
```

而相对`readNode()`以及`readIdx()`函数就是以上两个函数的逆操作，在这里不作解释。

### 2.2 B+树的运算实现 ###

#### 2.2.1 B+树插入的插入 ####

​	本次开发过程中使用的插入策略是递归的，层层递归找到插入位置，插入数据后对节点合法性进行判断，并进行相应的分裂操作，将这一层的的信息通过结构体 `retVal`返回到上一层，更新上一层的内容。主要由两个函数`treeInsert()` 以及 `treeInsertFirst()`完成。函数可以用伪代码表示：

```
插入(关键字, 数据， 起始节点){
	判断是否为空，如果是则直接在起始节点上插入，然后返回
	若不为空且为内部节点{
		利用二分查找找到进入下一层的位置
		读取这个位置上的节点
		retVal = 插入（关键字，数据，下一层上的节点）
		利用retVal的信息更新这一层
		判断这一层是否需要更新，如需要就执行相应操作
		如果被更新则写入磁盘
		返回这一层的retVal信息
	}
	若不为空且为叶节点{
        利用二分查找找到插入位置并插入
        判断节点是否合法，如果不合法就执行分裂操作
       	写入磁盘
       	返回这一层的retVal信息
	}
}
```

在执行插入操作时我们调用了两个函数，这是由于我的关键字与文件指针地址一一对应，所以插入比最小元素更小的元素时调用`treeInsert()`会解体(~~事实上是因为设计失误并且我不想改。。。~~ )所以就写了另一个`treeInsertFirst()`函数来弥补，但是总体性能还是不受影响的。

#### 2.2.2 B+树的删除####

​	删除和插入的想法基本一致，但是略有不同，插入递归到叶节点，但删除只能递归到根节点上一层的位置，通过`treeRomve()`函数完成，也可以用伪代码来表示。

```
删除（关键字，起始节点）{
    判断树是否为空，空则直接返回
    若起始节点为叶节点{  //这种情况只会在刚开始树上只有一个节点的情况下才会产生
    	利用二分查找找到删除位置并执行删除
    	返回
    } 
    利用二分查找找到进入下一层的位置，读取这个节点
    若读进来的节点是叶节点{
        利用二分查找找到删除位置
       	删除对应节点，并把这个位置记录在可用位置的队列里
       	检查节点合法性，若不合法则执行相应操作
       	如果需要的话 更新当前起始节点
       	将叶节点写入文件
       	将当前节点写入文件
       	利用一个整型返回当前节点执行了什么操作给上一层
    }
    若读进来的节点是内部节点{
        返回值 = 删除(关键字，起始结点) //递归进入下一层
        如果返回值表明当前节点的儿子执行了合并或者借节点的操作{
            如果借了节点 并且 导致了当前节点需要更新{
            	利用还在内存里的儿子更新当前节点
            	将当前节点写入磁盘
            }
            
           如果合并了节点{
               更新当前节点 //由于当前节点的儿子上发生了合并，所以当前节点的规模减少1,所以也需要验证是否需要进行相关操作
               检查节点合法性，若不合法则执行相应操作
               将当前节点写入磁盘
               根据当前节点的进行的操作返回一个值
           }
        }
        如果没有，则直接返回代表什么都没有做的值
    }
}
```



#### 2.2.3 分裂节点 #### 

分裂节点的函数在本次B+树的实现中起到了非常重要的作用。

主要是`splitNode()`函数，这个函数维护了同一层的链以及分类根节点时树的增高与更新，伪代码如下：

```
分裂节点(目标节点){
    如果有的话，读取前驱节点
    如果有的话，读取后继节点
    分配新的节点
    拷贝传入目标节点的一半
    如果需要的话 维护本层的链
   	写入磁盘
   	如果传入的目标节点是根节点{
        建立新节点作为根
        更新B+树信息，写入磁盘。
   	}
   	返回一个retVal配合treeInsert()使用
}
```



#### 2.2.4 借节点####

很简单的函数

```
借节点(目标节点){
    读取后继，判断是否可以接，若可以{
        从后继节点借一个儿子
        返回
    }
    读取前驱，判断是否可以借，若可以{
        从前驱节点借一个儿子
        返回
    }
}
```



## 三.开发以及结构优势 ##

* 由于在设计上做了很好的考虑，所以B+树的性能非常优秀。并且由于在`split()`函数中作了处理，使得每层都有一条水平链，可以支持区间查找以及其他一些更复杂的查找。
* 由于在读写节点时做了4K优化，所以性能十分优秀
* 合并了关键字和文件位置指针，使得开发变得容易
* 由于在删除节点后使用了队列以及相关文件记录已占用空间中可用区域的起始位置，所以在正常情况下占用的空间增长速度较慢，空间利用效率高。



# 数据库开发文档 #

只是给B+树加了个壳来兼容后端业务大爷的接口，告辞。





