package com.dingtao.rrmmp.presenter;
import com.dingtao.common.bean.Result;
import com.dingtao.common.bean.shop.HomeList;
import com.dingtao.common.core.WDPresenter;
import com.dingtao.common.core.DataCall;
import com.dingtao.common.core.http.NetworkManager;

import io.reactivex.Observable;

/**
 * @author dingtao
 * @date 2018/12/28 11:23
 * qq:1940870847
 */
public class HomeListPresenter extends WDPresenter<Result<HomeList>> {

    public HomeListPresenter(DataCall dataCall) {
        super(dataCall);
    }

    @Override
    protected String getModel(Object... args) {
        return NetworkManager.instance().get("commodity/v1/commodityList");
    }


}
