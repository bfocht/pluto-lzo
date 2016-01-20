using ManagedLZO;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace pluto_lzo
{
  class Program
  {
    static void Main(string[] args)
    {
      try
      {
        if (args.Length > 0)
        {
          foreach (var arg in args)
          {
            inflate(arg);
          }
        }
        else
          Console.WriteLine("No file provided");
      }
      catch (Exception ex)
      {
        Console.Write(ex);
      }
    }

    public static void compress()
    {

      string u = "D:\\projects\\managedlzo\\d_binarylight1.json";
      string o = "D:\\projects\\managedlzo\\d_binarylight1.json.lzo";
      byte[] input = File.ReadAllBytes(u);
      byte[] destination;


      MiniLZO.Compress(input, out destination);
      File.WriteAllBytes(o, destination);

    }


    public static void inflate(string filename)
    {
      string o = filename.Replace(".lzo", string.Empty);

      byte[] input = File.ReadAllBytes(filename);

      int size = 0;
      int header = 4;
      for (int i = 0; i < header; i++)
      {
        byte b1 = input[i];
        size += b1 * (int)Math.Pow(256, i);
      }

      byte[] destination = new byte[size];
      byte[] buffer = new byte[input.Length - header];
      Buffer.BlockCopy(input, header, buffer, 0, buffer.Length);

      MiniLZO.Decompress(buffer, destination);
      File.WriteAllBytes(o, destination);
    }
  }
}
