# JNI-C语言网络请求+MVP框架

<img src="https://github.com/VcStrong/KotlinMVPDemo/blob/master/image/1.jpg" width="300" align=center /><img src="https://github.com/VcStrong/KotlinMVPDemo/blob/master/image/2.jpg" width="300" align=center /><img src="https://github.com/VcStrong/KotlinMVPDemo/blob/master/image/3.jpg" width="300" align=center /><img src="https://github.com/VcStrong/KotlinMVPDemo/blob/master/image/4.jpg" width="300" align=center />
<br/>
<br/>

Rxjava+Retrofit版mvp参见：https://github.com/VcStrong/RxRetrofitMVPDemo.git<br/>
kotlin版mvp参见：https://github.com/VcStrong/KotlinMVPDemo.git<br/>

#### 注：未实现上传文件，目前仅支持发送普通请求。
本项目采用C语言 socket完成http请求，代码在common包下cpp里面；
解决了socket只能read超时才能完成请求的问题；解决了http响应内容任意大小的问题。
这两个问题记录在博客：C 基于socket发送http请求https://blog.csdn.net/VcStrong/article/details/103389243

### 框架包含以下
- androidx：这个系列的jar包和appcompat.support对立的，参见谷歌官方文档
- rxjava2
- greendao：数据库如果要加密，请配合SqlCipher使用，参见：https://blog.csdn.net/VcStrong/article/details/82972043
- MZBanner：banner如果不需要刻意去掉。
- fresco：图片加载
- xRecyclerView
- easypermissions：权限申请比较好用
- Arouter

