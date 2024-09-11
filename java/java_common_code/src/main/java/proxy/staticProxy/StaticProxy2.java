package proxy;

interface IUserDao {
    public void save();
}

class UserDao implements IUserDao {
    @Override
    public void save() {
        System.out.println("保存数据...");
    }
}

// 代理对象
class UserDaoProxy implements IUserDao {
    private IUserDao target; // note 定义一个接口引用的成员变量

    public UserDaoProxy(IUserDao target) {
        this.target = target;
    }

    @Override
    public void save() {
        System.out.println("开启事务"); // 扩展功能
        target.save();
        System.out.println("关闭事务");
    }
}

public class StaticProxy2 {
    public static void main(String[] args) {
        UserDaoProxy userDaoProxy = new UserDaoProxy(new UserDao());
        userDaoProxy.save();
    }
}

