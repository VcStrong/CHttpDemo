package com.dingtao.rrmmp.presenter;

import com.dingtao.common.bean.Result;
import com.dingtao.common.core.WDPresenter;
import com.dingtao.common.core.DataCall;
import com.dingtao.common.core.http.NetworkManager;

import java.util.List;

import io.reactivex.Observable;

/**
 * @author dingtao
 * @date 2018/12/28 11:23
 * qq:1940870847
 */
public class GreatPresenter extends WDPresenter<Result> {

    public GreatPresenter(DataCall consumer) {
        super(consumer);
    }

    @Override
    protected String getModel(Object... args) {
        return NetworkManager.instance().post("circle/verify/v1/addCircleGreat",
                new String[]{"circleId"},
                new String[]{String.valueOf((long)args[2])},
                new String[]{"userId","sessionId"},
                new String[]{(String)args[0],(String)args[1]});
    }


}
