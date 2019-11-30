package com.dingtao.rrmmp.presenter;
import com.dingtao.common.bean.Result;
import com.dingtao.common.bean.UserInfo;
import com.dingtao.common.core.WDPresenter;
import com.dingtao.common.core.DataCall;
import com.dingtao.common.core.http.NetworkManager;

import io.reactivex.Observable;

/**
 * @author dingtao
 * @date 2018/12/28 11:23
 * qq:1940870847
 */
public class LoginPresenter extends WDPresenter<Result<UserInfo>> {

    public LoginPresenter(DataCall dataCall) {
        super(dataCall);
    }

    @Override
    protected String getModel(Object... args) {
        return NetworkManager.instance().post("user/v1/login",
                new String[]{"phone","pwd"},
                new String[]{(String)args[0],(String)args[1]});
    }


}
