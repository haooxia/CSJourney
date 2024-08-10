package singleton;

// note: 懒汉式单例模式(单线程有效 多线程出错)
public class LazyMan {
    // 将构造器私有化 防止直接new
    private LazyMan(){
        //System.out.println(Thread.currentThread().getName() + " OK");
    };
    // 创建一个私有的静态实例(不初始化)
    private static LazyMan lazyMan;
    // 提供一个公共的静态方法返回实例
    public static LazyMan getInstance() {
        if (lazyMan == null) { // note: 多个线程同时访问这个方法时，可能会创建多个实例
            lazyMan = new LazyMan();
        }
        return lazyMan;
    }

    //// 多线程并发
    //public static void main(String[] args) {
    //    for (int i=0; i<10; ++i) {
    //        new Thread(() -> {
    //            LazyMan.getInstance();
    //        }).start();
    //    }
    //}
}
