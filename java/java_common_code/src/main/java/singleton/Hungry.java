package singleton;

// note: 饿汉式单例模式
public class Hungry {
    // 1.私有化构造器 防止直接new
    private Hungry(){};
    // 2.私有化静态实例
    private final static Hungry hungry = new Hungry();
    // 3.暴露公共的静态方法返回实例
    public static Hungry getInstance() {
        return hungry;
    }
}
