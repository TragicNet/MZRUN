public class arrays {
  public static void main(String[] args) {
    String[] words = { "car", "bus", "van" };
    int[] nums = { 1, 4, 5, 3, 2 };
    words[2]  = "bike";
    System.out.println(words[2]);
    System.out.println("Number Array Length: " + nums.length);
    System.out.print("Words: ");
    for(String i : words) {
        System.out.print(i + " ");
    }
  }
}