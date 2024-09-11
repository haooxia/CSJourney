package proxy.staticProxy;

interface Marry {
    void marry();
}

// 真实对象
class XiaoMing implements Marry {
    @Override
    public void marry() {
        System.out.println("小明去结婚.");
    }
}

// 代理角色：婚庆公司
class Company implements Marry {
    // 要在代理类中调用目标类的方法，所以需要传入一个目标对象，然后用一个接口引用指向即可
    private Marry target;
    public Company(Marry target) {
        this.target = target;
    }
    @Override
    public void marry() {
        before();
        this.target.marry();
        after();
    }
    // 增强行为
    private void before() {
        System.out.println("结婚之前，布置现场");
    }
    private void after() {
        System.out.println("结婚之后，收尾款");
    }
}

public class StaticProxy {
    public static void main(String[] args) {
        // note: 之所以可以把真是对象传递给代理对象，是因为Company构造函数接受一个Marry类型的参数，你可以传递任何实现了Marry接口的对象
        //  本质上是接口多态：接口类型的引用指向实现类的对象，以及接口引用调用实现类中的方法
        XiaoMing xm = new XiaoMing();
        Company company = new Company(xm);
        // 接口引用调用实现类的方法
        company.marry();
    }
}

