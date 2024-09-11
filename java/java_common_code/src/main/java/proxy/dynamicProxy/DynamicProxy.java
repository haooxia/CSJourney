package proxy.dynamicProxy;

// 动态代理借助jdk api

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

interface IUserDao {
    public void save();
}

// note: 目标对象需要实现interface，代理对象无需
class UserDao implements IUserDao {
    @Override
    public void save() {
        System.out.println("保存数据...");
    }
}

class ProxyFactory { // 无需实现interface
    private Object target; // 维护一个目标对象

    public ProxyFactory(Object target) {
        this.target = target;
    }
    // note: 为目标对象生成代理对象
    public IUserDao getProxyInstance() {

        return (IUserDao) Proxy.newProxyInstance( // 注意返回值强转一下
                target.getClass().getClassLoader(), // 类加载器 固定
                target.getClass().getInterfaces(), // 目标对象实现的接口的类型
                new InvocationHandler() { // 匿名内部类
                    @Override
                    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
                        // 在代理对象上调用方法并返回结果
                        System.out.println("开启事务");
                        // 执行目标对象方法
                        Object returnValue = method.invoke(target, args);
                        System.out.println("提交事务");
                        return null;
                    }
                }
        );

    }
}

public class DynamicProxy {
    public static void main(String[] args) {
        // 注意要强制类型转换: Object -> IUserDao
        IUserDao proxyInstance = new ProxyFactory(new UserDao()).getProxyInstance();
        proxyInstance.save();
    }
}
