using Group19LibraryApp.components;
using Group19LibraryApp.Pages.Components;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

namespace Group19LibraryApp.Data
{
     public class LibraryData
    {
        private bool initalized = false;
        public bool STATE = false;
        public static List<LibraryItem> books = new List<LibraryItem>();
        public static List<LibraryItem> history = new List<LibraryItem>();

        //student book data
        public static Dictionary<String, String> studentBooks = new Dictionary<string, string> { { "Networking for Dummies", "CPSC 441" }, { "Principles of Economics", "ECON 201" }, { "American Psycho", "LIT 101" } };
        public static string lastPage = "";
        public static String currentLocation = "LibraryA";

        public void InitalizeData()
        {
            if (initalized)
                return;
            initalized = true;
            System.Diagnostics.Debug.WriteLine("Reading library data file...");

            String content = File.ReadAllText("Data/Books.txt");
            content = content.Trim().Replace("\n", "").Replace("\r", "");
            String[] content2 = content.Split('}', StringSplitOptions.RemoveEmptyEntries);
            List<String[]> bookInfo = new List<String[]>();
            foreach(String s in content2)
            {
                string s2 = s.Replace("{", "");
                bookInfo.Add(s2.Split(';'));
                
            }
            
            foreach (string[] lines in bookInfo)
            { 
                LibraryItem processing = new LibraryItem();
                foreach (string line in lines)
                {
                    System.Diagnostics.Debug.WriteLine(line);
                    string trimLine = line.Trim();

                    string[] splitLine = trimLine.Split(':');
                    if(splitLine.Length != 2) { continue; }
                    //image
                    if (splitLine[0].Equals("Image"))
                        processing.ItemImageLink = splitLine[1];

                    //layout-image
                    if (splitLine[0].Equals("Layout Image"))
                        processing.ItemLocationImageLink = splitLine[1];

                    //Title
                    if (splitLine[0].Equals("Title"))
                        processing.Title = splitLine[1];

                    //Author
                    if (splitLine[0].Equals("Author"))
                        processing.Author = splitLine[1];

                    //Type
                    if (splitLine[0].Equals("Type"))
                        processing.Type = splitLine[1];

                    //Location
                    if (splitLine[0].Equals("Location"))
                    {
                        string[] localLine = splitLine[1].Split(',');
                        localLine[0] = localLine[0].TrimStart('<');
                        localLine[3] = localLine[3].TrimEnd('>');
                        processing.Location = localLine;
                    }

                    //synopsis
                    if (splitLine[0].Equals("Synopsis"))
                        processing.Synopsis = splitLine[1];

                    //Category
                    if (splitLine[0].Equals("Category"))
                    {
                        string[] tokens = splitLine[1].Split(",");
                        processing.Genre = tokens;
                    }
                }
                // Default settings
                processing.UnReserved = true;
                processing.LocationSaved = false;
                books.Add(processing);
            }
            System.Diagnostics.Debug.WriteLine("Finished reading data file, found " + books.Count + " books");
        }      
     }
}