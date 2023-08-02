using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using static System.Reflection.Metadata.BlobBuilder;

namespace Group19LibraryApp.Pages.Components
{
    public class LibraryCategory
    {
        [Required]
        public string CategoryName { get; set; }
        public string CategoryType { get; set; }
        public bool CategorySelected { get; set; } = false;

    }
}
