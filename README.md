# MySock5
基于epoll nonblock的简单sock5代理服务器

- bzero(), memset(), memcpy()。
	- memset是以字节为单位，初始化内存块，在初始化int等类型时，则需要注意
	- 当结构体类型中包含指针时，在使用memset初始化时需要小心
- c与c++的一点不同：c中没有类似c++中的函数参数引用传递，改为void*等
