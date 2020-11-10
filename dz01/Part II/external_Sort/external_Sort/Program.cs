using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace external_Sort
{
    class Program
    {
        static int numberOfFiles = 0;

        static void Merge(string[] key, string[] arr, int lowIndex, int middleIndex, int highIndex)
        {
            var left = lowIndex;
            var right = middleIndex + 1;
            string[,] tempArray = new string[highIndex - lowIndex + 1, 2];
            var index = 0;

            while ((left <= middleIndex) && (right <= highIndex))
            {
                if (key[left].CompareTo(key[right]) < 0)
                {
                    tempArray[index, 0] = "" + key[left];
                    tempArray[index, 1] = arr[left];
                    left++;
                }
                else
                {
                    tempArray[index, 0] = "" + key[right];
                    tempArray[index, 1] = arr[right];
                    right++;
                }

                index++;
            }

            for (var i = left; i <= middleIndex; i++)
            {
                tempArray[index, 0] = "" + key[i];
                tempArray[index, 1] = arr[i];
                index++;
            }

            for (var i = right; i <= highIndex; i++)
            {
                tempArray[index, 0] = "" + key[i];
                tempArray[index, 1] = arr[i];
                index++;
            }

            for (var i = 0; i < tempArray.GetLength(0); i++)
            {
                key[lowIndex + i] = tempArray[i, 0];
                arr[lowIndex + i] = tempArray[i, 1];
            }
        }

        static void Merge(int[] key, string[] arr, int lowIndex, int middleIndex, int highIndex)
        {
            var left = lowIndex;
            var right = middleIndex + 1;
            string[,] tempArray = new string[highIndex - lowIndex + 1,2];
            var index = 0;

            while ((left <= middleIndex) && (right <= highIndex))
            {
                if (key[left] < key[right])
                {
                    tempArray[index,0] = "" + key[left];
                    tempArray[index,1] = arr[left];
                    left++;
                }
                else
                {
                    tempArray[index,0] = "" + key[right];
                    tempArray[index,1] = arr[right];
                    right++;
                }

                index++;
            }

            for (var i = left; i <= middleIndex; i++)
            {
                tempArray[index,0] = "" + key[i];
                tempArray[index,1] = arr[i];
                index++;
            }

            for (var i = right; i <= highIndex; i++)
            {
                tempArray[index,0] = "" + key[i];
                tempArray[index,1] = arr[i];
                index++;
            }

            for (var i = 0; i < tempArray.GetLength(0); i++)
            {
                key[lowIndex + i] = int.Parse(tempArray[i,0]);
                arr[lowIndex + i] = tempArray[i,1];
            }
        }

        static string[] MergeSort(int[] key, string[] arr, int lowIndex, int highIndex)
        {
            if (lowIndex < highIndex)
            {
                var middleIndex = (lowIndex + highIndex) / 2;
                MergeSort(key, arr, lowIndex, middleIndex);
                MergeSort(key, arr, middleIndex + 1, highIndex);
                Merge(key, arr, lowIndex, middleIndex, highIndex);
            }

            return arr;
        }

        static string[] MergeSort(string[] key, string[] arr, int lowIndex, int highIndex)
        {
            if (lowIndex < highIndex)
            {
                var middleIndex = (lowIndex + highIndex) / 2;
                MergeSort(key, arr, lowIndex, middleIndex);
                MergeSort(key, arr, middleIndex + 1, highIndex);
                Merge(key, arr, lowIndex, middleIndex, highIndex);
            }

            return arr;
        }

        static void Split(string file)
        {
            int splitNum = 1;
            StreamWriter sw = new StreamWriter(string.Format("split{0:d5}.csv", splitNum));
            long readLine = 0;
            using (StreamReader sr = new StreamReader(file))
            {
                while (sr.Peek() >= 0)
                {
                    if (++readLine % 5000 == 0)
                        Console.Write("{0:f2}%   \r", 100.0 * sr.BaseStream.Position / sr.BaseStream.Length);

                    sw.WriteLine(sr.ReadLine());

                    if (sw.BaseStream.Length > 100000000 && sr.Peek() >= 0)
                    {
                        sw.Close();
                        splitNum++;
                        sw = new StreamWriter(string.Format("split{0:d5}.csv", splitNum));
                    }
                }
            }
            sw.Close();
            numberOfFiles = splitNum;
            Console.Write("{0:f2}%   \r", 100.0);
            Console.WriteLine();
            Console.WriteLine("File splitted!");
        }

        static string GetKey(string str, int key)
        {
            string[] strings = str.Split(',');
            int j = -1;
            bool isStr = false;
            int diff = 0;
            for (int i = 0; i < strings.Length; i++)
            {
                if (!isStr && strings[i].Length != 0 && strings[i][0] == '\"' && (strings[i][strings[i].Length - 1] != '\"' || strings[i].Length == 1))
                {
                    isStr = true;
                    diff = 0;
                }
                else if (isStr && strings[i].Length != 0 && strings[i][strings[i].Length - 1] != '\"')
                {
                    diff++;
                    strings[i - diff] += "," + strings[i];
                }
                else if (isStr && strings[i].Length != 0 && strings[i][strings[i].Length - 1] == '\"')
                {
                    j++;
                    diff++;
                    strings[i - diff] += "," + strings[i];
                    isStr = false;
                    if (j == key)
                    {
                        return strings[i - diff];
                    }
                }
                else if (strings[i].Length == 0)
                {
                    continue;
                }
                else
                {
                    j++;
                    if (j == key)
                    {
                        return strings[i];
                    }
                }
            }
            return "";
        }

        static void SortTheChunks(int keyN, string type)
        {
            int i = 0;
            foreach (string path in Directory.GetFiles(Directory.GetCurrentDirectory(), "split*.csv"))
            {
                string[] contents = File.ReadAllLines(path);
                if (type == "string")
                {
                    string[] keys = new string[contents.Length];
                    int j = 0;
                    foreach (var elem in contents)
                    {
                        keys[j++] = GetKey(elem, keyN);
                    }
                    MergeSort(keys, contents, 0, contents.Length - 1);
                    keys = null;
                }
                else if (type == "int")
                {
                    int[] keys = new int[contents.Length];
                    int j = 0;
                    foreach (var elem in contents)
                    {
                        if (!int.TryParse(GetKey(elem, keyN), out keys[j]))
                        {
                            keys[j] = 1000; // на всякий случай, чтобы не падало
                            Console.WriteLine(elem); // вывести ошибочную строку
                            Console.WriteLine(type);
                            Console.WriteLine(keyN);
                        }
                        j++;
                    }
                    MergeSort(keys, contents, 0, contents.Length - 1);
                    keys = null;
                }
                else if (type == "float")
                {
                    double[] keys = new double[contents.Length];
                    int j = 0;
                    foreach (var elem in contents)
                    {
                        if (!double.TryParse(GetKey(elem, keyN), out keys[j]))
                        {
                            keys[j] = 1.0; // на всякий случай, чтобы не падало
                            Console.WriteLine(elem); // вывести ошибочную строку
                        }
                        j++;
                    }
                    Array.Sort(keys, contents);
                    keys = null;
                }

                string newpath = path.Replace("split", "sorted");
                File.WriteAllLines(newpath, contents);
                File.Delete(path);
                contents = null;
                Console.Write("{0:f2}%   \r", 100.0 * (i + 1) / numberOfFiles);
                GC.Collect();
                i++;
            }
            Console.Write("{0:f2}%   \r", 100.0);
            Console.WriteLine();
            Console.WriteLine("Files sorted!");
        }

        static void MergeTheChunks(int keyN, string type, string filename)
        {
            string[] paths = Directory.GetFiles(Directory.GetCurrentDirectory(), "sorted*.csv");
            int chunks = paths.Length;
            int recordsize = 100; 
            int records = 8000000; 
            int maxusage = 600000000;
            int buffersize = maxusage / chunks; 
            int bufferlen = (int)(buffersize / recordsize / 7.5); 

            StreamReader[] readers = new StreamReader[chunks];
            for (int i = 0; i < chunks; i++)
                readers[i] = new StreamReader(paths[i]);

            Queue<string>[] queues = new Queue<string>[chunks];
            for (int i = 0; i < chunks; i++)
                queues[i] = new Queue<string>(bufferlen);

            for (int i = 0; i < chunks; i++)
                LoadQueue(queues[i], readers[i], bufferlen);

            StreamWriter sw = new StreamWriter(filename + "_sorted.csv", false);
            bool done = false;
            int lowest_index, j, progress = 0;
            string lowest_value, newStr;
            while (!done)
            {
                if (++progress % 5000 == 0)
                    Console.Write("{0:f2}%   \r", 100.0 * progress / records);

                lowest_index = -1;
                lowest_value = "";
                for (j = 0; j < chunks; j++)
                {
                    if (queues[j] != null)
                    {
                        newStr = GetKey(queues[j].Peek(), keyN);
                        if (type == "string" && (lowest_index < 0 || String.Compare(newStr, lowest_value) < 0))
                        {
                            lowest_index = j;
                            lowest_value = newStr;
                        }
                        else if (type == "int" && (lowest_index < 0 || int.Parse(newStr) < int.Parse(lowest_value)))
                        {
                            lowest_index = j;
                            lowest_value = newStr;
                        }
                        else if (type == "float" && (lowest_index < 0 || float.Parse(newStr) < float.Parse(lowest_value)))
                        {
                            lowest_index = j;
                            lowest_value = newStr;
                        }
                    }
                }

                if (lowest_index == -1) { done = true; break; }

                sw.WriteLine(queues[lowest_index].Peek());

                queues[lowest_index].Dequeue();
                if (queues[lowest_index].Count == 0)
                {
                    LoadQueue(queues[lowest_index], readers[lowest_index], bufferlen);
                    if (queues[lowest_index].Count == 0)
                    {
                        queues[lowest_index] = null;
                    }
                }
            }
            sw.Close();

            for (int i = 0; i < chunks; i++)
            {
                readers[i].Close();
                File.Delete(paths[i]);
            }
            Console.Write("{0:f2}%   \r", 100.0);
            Console.WriteLine();
            Console.WriteLine("Done!");
        }

        static void LoadQueue(Queue<string> queue, StreamReader file, int records)
        {
            for (int i = 0; i < records; i++)
            {
                if (file.Peek() < 0) break;
                queue.Enqueue(file.ReadLine());
            }
        }

        static void Main(string[] args)
        {
            System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");

            string filename = args[0];
            int numberOfCol = int.Parse(args[1]);
            string type;
            if (args.Length >= 3)
                type = args[2];
            else
                type = "string";

            Split(filename + ".csv");
            SortTheChunks(numberOfCol - 1, type);
            MergeTheChunks(numberOfCol - 1, type, filename);

            Console.ReadLine();
        }
    }
}
