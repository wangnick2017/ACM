using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Clr;

namespace ACM.Pages
{
    /// <summary>
    /// PgMyTicket.xaml 的交互逻辑
    /// </summary>
    public partial class PgMyTicket : UserControl
    {
        MainWindow parentWindow;
        ClrBackend clr;
        public PgMyTicket(MainWindow parent, ClrBackend c)
        {
            InitializeComponent();
            parentWindow = parent;
            clr = c;
        }

        public void Clear()
        {
            date1.SelectedDate = date2.SelectedDate = DateTime.Today;
            foreach (CheckBox ch in stack.Children)
            {
                ch.IsChecked = false;
            }
            lst.Items.Clear();
        }

        private void BtnQuery_Click(object sender, RoutedEventArgs e)
        {
            string catalog = "";
            foreach (CheckBox ch in stack.Children)
            {
                if (ch.Content.ToString().Length == 1 && ch.IsChecked == true)
                {
                    catalog += ch.Content;
                }
            }
            List<Ticket> list = new List<Ticket>();
            for (DateTime d = (DateTime)date1.SelectedDate, de = (DateTime)date2.SelectedDate; d <= de; d = d.AddDays(1))
            {
                list.AddRange(clr.QueryOrder(parentWindow.user.ID, d.ToString("yyyy-MM-dd"), catalog));
            }
            Something.Merge(list);
            lst.Items.Clear();
            if (list != null && list.Count > 0)
                foreach (Ticket t in list)
                {
                    lst.Items.Add(new ListItem(t, clr, parentWindow.user.ID, 1));
                }
        }

        private void BtnBack_Click(object sender, RoutedEventArgs e)
        {
            parentWindow.GotoUser(-1);
        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            foreach (CheckBox ch in stack.Children)
            {
                ch.IsChecked = true;
            }
        }
    }
}
