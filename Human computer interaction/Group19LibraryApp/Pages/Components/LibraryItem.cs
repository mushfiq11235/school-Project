using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.ComponentModel.DataAnnotations;
using System.Net.NetworkInformation;
using Group19LibraryApp.Data;

namespace Group19LibraryApp.Pages.Components
{
    public class LibraryItem
    {
        private static LibraryData data;
        private static LibraryItem reservedList;
        private static LibraryItem allItems;

        [Required]
        public string Title { get; set; } = "";
        public string Author { get; set; } = "";
        public string Type { get; set; } = "";
        public string Synopsis { get; set; } = "";
        public string[] Genre { get; set; } = { };
        public string? ItemImageLink { get; set; } = "imageNotFound.jpg";
        public string[] Location { get; set; } = { "NO LOCATION" };
        public string[] WhereToPickUp { get; set; } = { "NO LOCATION" };
        public string? ItemLocationImageLink { get; set; }
        public int Waitlist_Position { get; set; } = 0;

        public string Status { get; set; } = "";

        public bool IsReserved { get; set; } = false; // ?
        public bool UnReserved { get; set; } = true;
        public bool LocationSaved { get; set; } = false;

        /*
        public LibraryItem ReservedItems { Replace string with LibraryItem[] after testing
            get {
                if (reservedList != null) {
                    return reservedList;
                }
                else {
                    data.
                }
            }

            set {

            }

        }
        */
    }

}
