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
    /// PgTrains.xaml 的交互逻辑
    /// </summary>
    public partial class PgTrains : UserControl
    {
        MainWindow parentWindow;
        ClrBackend clr;
        public PgTrains(MainWindow parent, ClrBackend c)
        {
            InitializeComponent();
            parentWindow = parent;
            clr = c;
        }

        public void Clear()
        {
            txtID.Clear();
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            parentWindow.GotoAdmin(1);
        }

        private void btnQuery_Click(object sender, RoutedEventArgs e)
        {
            if (txtID.Text == "" || txtID.Text.Contains(" "))
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrains.failed") as string);
                dg.ShowDialog();
                return;
            }
            Train t = clr.QueryTrain(txtID.Text);
            if (t.trainID == "0")
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrains.failed") as string);
                dg.ShowDialog();
                return;
            }
            parentWindow.GotoTrain(0, t);
        }

        private void btnNew_Click(object sender, RoutedEventArgs e)
        {
            parentWindow.GotoTrain(1);
        }
    }
}
