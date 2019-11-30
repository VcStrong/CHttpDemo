package com.dingtao.common.core;

import android.os.Handler;

import com.dingtao.common.bean.BDResult;
import com.dingtao.common.bean.Result;
import com.dingtao.common.core.exception.ApiException;
import com.dingtao.common.core.exception.CustomException;
import com.dingtao.common.core.http.NetworkManager;
import com.google.gson.Gson;

import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.FutureTask;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import io.reactivex.Observable;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.Disposable;
import io.reactivex.functions.Action;
import io.reactivex.functions.Consumer;
import io.reactivex.functions.Function;
import io.reactivex.schedulers.Schedulers;

/**
 * @author dingtao
 * @date 2018/12/28 11:30
 * qq:1940870847
 */
public abstract class WDPresenter<T> {
    public final static int REQUEST_TYPE_DEFAULT = 0;//默认IRquest
    public final static int REQUEST_TYPE_APP_ORDER = 1;//例：这个为订单请求接口，由于接口类中方法太多，所以写了另外一个业务接口
    public final static int REQUEST_TYPE_APP_AD = 2;//例：这个为广告请求接口，由于接口类中方法太多，所以写了另外一个业务接口
    public final static int REQUEST_TYPE_SDK_BD = 100;//例：这个为请求百度的接口，请求结构为另外一种

    public final static int RESPONSE_TYPE_DEFAULT = 0;
    public final static int RESPONSE_TYPE_SDK_BD = 100;////例：这个为请求百度的接口，接口结构为另外一种

    private DataCall dataCall;
    private final static Gson gson = new Gson();

    private boolean running;//是否运行，防止重复请求，这里没有用rxjava的重复过滤（个人感觉重复过滤用在多次点击上比较好，请求从体验角度最好不要间隔过滤）
    private Disposable disposable;//rxjava层取消请求

    public WDPresenter(DataCall dataCall) {
        this.dataCall = dataCall;
    }

    protected abstract String getModel(Object... args);

    public void reqeust(final Object... args) {
        if (running) {
            return;
        }

        running = true;
        Observable observable = Observable.fromCallable(new Callable<T>() {
            @Override
            public T call() throws Exception {
                String strResult = getModel(args);
                T t = null;
                try {
                    t = gson.fromJson(strResult, getTType());
                }catch (Exception e){
                    e.printStackTrace();
                    throw new ApiException("继承WDPresenter未加泛型或者解析错误\n"+e.getMessage());
                }
                return t;
            }
        });

        disposable = observable
                .doOnError(new Consumer<Throwable>() {
                    @Override
                    public void accept(Throwable e) throws Exception {
                        running = false;
                        dataCall.fail(CustomException.handleException(e), args);
                    }
                })
                .onErrorReturn(new Function<Throwable,Throwable>() {
                    @Override
                    public Throwable apply(Throwable o) throws Exception {
                        return o;
                    }
                })
                .subscribeOn(Schedulers.io())//将请求调度到子线程上
                .observeOn(AndroidSchedulers.mainThread())//观察响应结果，把响应结果调度到主线程中处理
                .subscribe(getConsumer(args), new Consumer<Throwable>() {
                    @Override
                    public void accept(Throwable e) throws Exception {
                        running = false;
                        dataCall.fail(CustomException.handleException(e), args);
                    }
                });
    }

    /**
     * 获取泛型对相应的Class对象
     * @return Type
     */
    private Type  getTType(){
        //返回表示此 Class 所表示的实体（类、接口、基本类型或 void）的直接超类的 Type。
        ParameterizedType type = (ParameterizedType)getClass().getGenericSuperclass();
        //返回表示此类型实际类型参数的 Type 对象的数组()，想要获取第二个泛型的Class，所以索引写1
        Type[] types = type.getActualTypeArguments();
        return types[0];
    }


    /**
     * 根据返回值灵活改变Consumer或者自己直接重写都可以
     */
    protected Consumer getConsumer(final Object... args) {
        return new Consumer<T>() {
            @Override
            public void accept(T r) throws Exception {
                running = false;
                if (r instanceof BDResult) {
                    BDResult result = (BDResult) r;
                    if (result.getCode() == 0) {
                        dataCall.success(result.getData(), args);
                    } else {
                        dataCall.fail(new ApiException(String.valueOf(result.getCode()), result.getMsg()));
                    }
                } else if (r instanceof Result) {
                    Result result = (Result) r;
                    if (result.getStatus().equals("0000")) {
                        dataCall.success(result.getResult(), args);
                    } else {
                        dataCall.fail(new ApiException(result.getStatus(), result.getMessage()));
                    }
                }
            }
        };
    }

    /**
     * 请求类型，方便修改不同的IRequest接口和Rtrofit
     */
    protected int getRequestType() {
        return REQUEST_TYPE_DEFAULT;
    }

    /**
     * 返回值类型，方便多接口，应对大项目多数据结构
     */
    protected int getResponseType() {
        return RESPONSE_TYPE_DEFAULT;
    }

    /**
     * 取消请求
     */
    public void cancelRequest() {
        running = false;
        if (disposable != null) {
            disposable.dispose();
        }
    }

    /**
     * 是否正在运行
     */
    public boolean isRunning() {
        return running;
    }

    /**
     * 解绑
     */
    public void unBind() {
        dataCall = null;
    }
}
