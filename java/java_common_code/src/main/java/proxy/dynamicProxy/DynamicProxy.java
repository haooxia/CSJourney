package proxy;

// 动态代理借助jdk api

interface IUserDao_ {
    public void save();
}

class UserDao_ implements IUserDao_ {

}

interface Star {
    String sing(String name);
    void dance();
}

class BigStar implements Star {
    private String name;
    public BigStar(String name) {this.name = name;}
    public String sing(String name) {
        System.out.println(this.name + "正在唱: " + name);
        return "谢谢";
    }
    public void dance() {
        System.out.println(this.name + "正在跳舞!");
    }
}



public class DynamicProxy {

}
