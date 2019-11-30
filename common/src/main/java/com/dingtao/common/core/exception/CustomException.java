package com.dingtao.common.core.exception;

import android.net.ParseException;

import com.google.gson.JsonParseException;

import org.json.JSONException;

import java.net.ConnectException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

/**
 * @author dingtao
 * @date 2019/1/2 7:03 PM
 * 异常处理工具类：你把异常传给我，哪些异常需要页面Toast提示或者操作
 */
public class CustomException {

    /**
     * 未知错误
     */
    public static final String UNKNOWN = "1000";

    /**
     * 解析错误
     */
    public static final String PARSE_ERROR = "1001";

    /**
     * 网络错误
     */
    public static final String NETWORK_ERROR = "1002";

    /**
     * 协议错误
     */
    public static final String HTTP_ERROR = "1003";

    /**
     * 处理系统异常，封装成ApiException
     * Throwable包含Error和Exception
     */
    public static ApiException handleException(Throwable e) {
        e.printStackTrace();//打印异常
        if (e instanceof JsonParseException
                || e instanceof JSONException
                || e instanceof ParseException) {
            //解析错误
            return new ApiException(PARSE_ERROR, "解析异常");
        } else if (e instanceof ConnectException  || e instanceof UnknownHostException || e instanceof SocketTimeoutException) {
            //网络错误
            return new ApiException(NETWORK_ERROR, e.getMessage());
        } else if(e instanceof ApiException){
            return (ApiException) e;
        } else {
            //未知错误
            return new ApiException(UNKNOWN, e.getMessage());
        }
    }
}
