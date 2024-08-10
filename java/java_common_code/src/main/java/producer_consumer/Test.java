package producer_consumer;

import java.util.*;

class Student {
    String name;
    int age;

    public Student(String name, int age) {
        this.name = name;
        this.age = age;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Student student = (Student) o;
        return age == student.age && Objects.equals(name, student.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, age);
    }
}

public class Test {
    public static void main(String[] args) throws InterruptedException {

        Student t1 = new Student("John", 20);
        Student t2 = new Student("John", 20);
        Student t3 = new Student("hello", 20);

        System.out.println(t1.hashCode());
        System.out.println(t2.hashCode());
        System.out.println(t3.hashCode());

        System.out.println(t1.equals(t2));
        System.out.println(t1.equals(t3));

        System.out.println(t1 == t2);
    }
}
