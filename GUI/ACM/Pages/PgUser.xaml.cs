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
    /// PgUser.xaml 的交互逻辑
    /// </summary>
    public partial class PgUser : UserControl
    {
        MainWindow parentWindow;
        ClrBackend clr;
        public PgUser(MainWindow parent, ClrBackend c)
        {
            InitializeComponent();
            parentWindow = parent;
            clr = c;
            
        }

        public void Clear()
        {
            lst.Items.Clear();
            loc1.Clear();
            loc2.Clear();
            date.SelectedDate = DateTime.Today;
        }

        int mode;
        public void SetMode(int m)
        {
            mode = m;
            btnBack.ToolTip = mode == 0 ? (TryFindResource("pguser.backTip0") as string) : (TryFindResource("pguser.backTip1") as string);
            BtnRefresh_Click(null, null);
            date.SelectedDate = DateTime.Today;
            SetInformation();
        }

        public void SetInformation()
        {
            txtName.Text = parentWindow.user.name;
            labID.Content = "ID: " + parentWindow.user.ID;
            labEmail.Content = "Email: " + parentWindow.user.email;
            labPhone.Content = "Phone: " + parentWindow.user.phone;
        }

        private void BtnBack_Click(object sender, RoutedEventArgs e)
        {
            if (mode==0)
            {
                parentWindow.GotoLogin();
            }
            else
            {
                parentWindow.GotoAdmin(1);
            }
        }

        private void BtnQuery_Click(object sender, RoutedEventArgs e)
        {
            parentWindow.GotoQuery(loc1.Text, loc2.Text, date.SelectedDate);
        }

        private void BtnMyTicket_Click(object sender, RoutedEventArgs e)
        {
            parentWindow.GotoMyTicket();
        }

        private void BtnRefresh_Click(object sender, RoutedEventArgs e)
        {
            List<Ticket> list = clr.QueryOrder(parentWindow.user.ID, DateTime.Today.ToString("yyyy-MM-dd"), "GDCZTKO");
            Something.Merge(list);
            lst.Items.Clear();
            if (list != null)
                for (int i = 0, s = list.Count; i < s; ++i)
                {
                    lst.Items.Add(new ListItem(list[i], clr, parentWindow.user.ID, 1));
                }
        }

        private void BtnProfile_Click(object sender, RoutedEventArgs e)
        {
            parentWindow.GotoProfile();
        }
    }
}
