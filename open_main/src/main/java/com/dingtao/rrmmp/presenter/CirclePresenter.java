package com.dingtao.rrmmp.presenter;

import com.dingtao.common.bean.Circle;
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
public class CirclePresenter extends WDPresenter<Result<List<Circle>>> {

    private int page=1;

    public int getPage() {
        return page;
    }

    public CirclePresenter(DataCall consumer) {
        super(consumer);
    }

    @Override
    protected String getModel(Object... args) {
        boolean refresh = (boolean)args[0];
        if (refresh){
            page = 1;
        }else{
            page++;
        }
        return NetworkManager.instance().get("circle/v1/findCircleList",
                new String[]{"page","count"},
                new String[]{String.valueOf(page),"20"},
                new String[]{"userId","sessionId"},
                new String[]{String.valueOf((long)args[1]),(String)args[2]});
    }


}
