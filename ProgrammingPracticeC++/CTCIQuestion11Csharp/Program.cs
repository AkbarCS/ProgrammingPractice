public class MainClass
{
    static void Main(string[] args)
    {
        Console.WriteLine("Input String");

        var inputString = Console.ReadLine();

        var characters = new List<char>();

        foreach (var character in inputString)
        {
            if (characters.Contains(character))
            {
                Console.WriteLine("String does not contain all unique characters");
                return;
            }

            characters.Add(character);
        }

        Console.WriteLine("String has all unique characters");
    }
}