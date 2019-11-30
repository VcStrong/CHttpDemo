package com.dingtao.common.core.http;

import com.dingtao.common.bean.UserInfo;
import com.dingtao.common.core.WDApplication;
import com.dingtao.common.core.WDPresenter;
import com.dingtao.common.core.db.DaoMaster;
import com.dingtao.common.core.db.UserInfoDao;
import com.dingtao.common.util.LogUtils;
import com.dingtao.common.util.StringUtils;

import java.io.IOException;
import java.util.List;
import java.util.concurrent.TimeUnit;

import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Retrofit;
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * @author dingtao
 * @date 2018/12/28 10:07
 * qq:1940870847
 */
public class NetworkManager {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * post和get方法中参数对应的类型，PARAM代表http请求体
     */
    public final static int PARAM = 0;
    /**
     * post和get方法中参数对应的类型，HEADER代表http请求头
     */
    public final static int HEADER = 1;

    private static NetworkManager instance;

    private NetworkManager() {

    }

    public static NetworkManager instance() {
        if (instance == null) {
            instance = new NetworkManager();
        }
        return instance;
    }

    public String post(String url) {
        return post(url, null, null, null, null);
    }

    /**
     * @param url           请求链接
     * @param headerOrParam @link如果参数是header，则为true，如果是param，则为false
     * @param keys
     * @param values
     * @return
     */
    public String post(String url, int headerOrParam, String[] keys, String[] values) {
        if (headerOrParam == HEADER)
            return post(url, null, null, keys, values);
        else
            return post(url, keys, values, null, null);
    }

    /**
     * @param url    请求链接
     * @param keys   请求参数
     * @param values 请求值
     * @return
     */
    public String post(String url, String[] keys, String[] values) {
        return post(url, keys, values, null, null);
    }

    public String post(String url, String[] pk, String[] pv, String[] hk, String[] hv) {
        StringBuilder headers = new StringBuilder();
        if (hk != null) {
            headers.append("\r\n");
            for (int i = 0; i < hk.length; i++) {
                headers.append(hk[i]);
                headers.append(":");
                headers.append(hv[i]);
                if (i != hk.length - 1) {
                    headers.append("\r\n");
                }
            }
        }
        if (pk != null) {// 请求参数判断
            StringBuilder params = new StringBuilder();
            for (int i = 0; i < pk.length; i++) {
//                LogUtils.i("key:" + pk[i] + "    values:" + pv[i]);
                params.append(pk[i]);
                params.append("=");
                params.append(pv[i]);
                if (i != pk.length - 1) {
                    params.append("&");
                }
            }
            String p = params.toString();
            LogUtils.i("请求方式为POST；请求头为："+headers.toString()+";请求参数为：" + p);
            return postByJNI(url, headers.toString(), p);
        } else {
            LogUtils.i("请求方式为POST；请求头为："+headers.toString()+";请求参数为：无");
            return postByJNI(url, headers.toString(), null);
        }
    }

    public String get(String url) {
        return get(url, null, null, null, null);
    }

    /**
     * @param url    请求链接
     * @param keys   请求参数
     * @param values 请求值
     * @return
     */
    public String get(String url, String[] keys, String[] values) {
        return get(url, keys, values, null, null);
    }

    /**
     * @param url           请求链接
     * @param headerOrParam @link如果参数是header，则为true，如果是param，则为false
     * @param keys
     * @param values
     * @return
     */
    public String get(String url, int headerOrParam, String[] keys, String[] values) {
        if (headerOrParam == HEADER)
            return post(url, null, null, keys, values);
        else
            return post(url, keys, values, null, null);
    }

    public String get(String url, String[] pk, String[] pv, String[] hk, String[] hv) {
        StringBuilder headers = new StringBuilder();
        if (hk != null) {
            headers.append("\r\n");
            for (int i = 0; i < hk.length; i++) {
                headers.append(hk[i]);
                headers.append(":");
                headers.append(hv[i]);
                if (i != hk.length - 1) {
                    headers.append("\r\n");
                }
            }
        }
        if (pk != null) {// 请求参数判断
            StringBuilder params = new StringBuilder();
            for (int i = 0; i < pk.length; i++) {
//                LogUtils.i("key:" + pk[i] + "    values:" + pv[i]);
                params.append(pk[i]);
                params.append("=");
                params.append(pv[i]);
                if (i != pk.length - 1) {
                    params.append("&");
                }
            }
            String p = params.toString();
            LogUtils.i("请求方式为GET；请求头为："+headers.toString()+";请求参数为：" + p);
            return getByJNI(url + "?" + p, headers.toString());
        } else {
            LogUtils.i("请求方式为GET；请求头为："+headers.toString()+";请求参数为：无");
            return getByJNI(url, headers.toString());
        }
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     * .baseUrl("http://169.254.101.220:8080/")//base_url:http+域名
     * //                .baseUrl("http://172.17.8.100/small/")//base_url:http+域名
     * .baseUrl("http://mobile.bwstudent.com/small/")//base_url:http+域名
     */
    public native String postByJNI(String url, String headers, String params);

    public native String getByJNI(String url, String headers);
}
