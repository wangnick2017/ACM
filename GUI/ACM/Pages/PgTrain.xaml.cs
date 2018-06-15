using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
    /// PageTrain.xaml 的交互逻辑
    /// </summary>
    public partial class PgTrain : UserControl
    {
        MainWindow parentWindow;
        ClrBackend clr;
        Train train, tmpTrain;
        DataTable table;
        public PgTrain(MainWindow parent, ClrBackend c)
        {
            InitializeComponent();
            parentWindow = parent;
            clr = c;
            table = new DataTable();
        }

        void SetTrain()
        {
            txtID.Text = train.trainID;
            txtName.Text = train.name;
            foreach (RadioButton rd in stack.Children)
            {
                rd.IsChecked = train.catalog.Contains(rd.Content.ToString());
            }
            foreach (CheckBox ch in stack2.Children)
            {
                ch.IsChecked = false;
                ch.IsChecked = train.namePrice.Contains(ch.Content.ToString());
            }

            for (int i = 0; i < train.numStation; ++i)
            {
                if (table.Rows.Count < i + 1)
                    table.Rows.Add();
                table.Rows[i][0] = train.stations[i].name;
                table.Rows[i][1] = train.stations[i].timeArrive;
                table.Rows[i][2] = train.stations[i].timeStart;
                table.Rows[i][3] = train.stations[i].timeStopover;
                for (int j = 0; j < train.numPrice; ++j)
                {
                    table.Rows[i][4 + j] = train.stations[i].price[j].ToString();
                }
            }
            grid.ItemsSource = table.AsDataView();
        }

        public void SetMode(int mode, Train t = null)
        {
            table.Columns.Clear();
            table.Columns.Add(TryFindResource("pgtrain.station") as string, typeof(string));
            table.Columns.Add(TryFindResource("pgtrain.arrive") as string, typeof(string));
            table.Columns.Add(TryFindResource("pgtrain.start") as string, typeof(string));
            table.Columns.Add(TryFindResource("pgtrain.stopover") as string, typeof(string));
            if (mode == 1)
            {
                title.Content = TryFindResource("pgtrain.add") as string;
                btnModify.Visibility = Visibility.Collapsed;
                btnSale.Visibility = Visibility.Collapsed;
                btnSubmit.Visibility = Visibility.Visible;
                btnDelete.Visibility = Visibility.Collapsed;
                txtID.IsEnabled = true;
                txtID.Text = "";
                txtName.IsEnabled = true;
                txtName.Text = "";
                foreach (RadioButton rd in stack.Children)
                {
                    rd.IsChecked = false;
                }
                foreach (CheckBox ch in stack2.Children)
                {
                    ch.IsChecked = false;
                }
                grid.IsReadOnly = false;
                stack.IsEnabled = true;
                stack2.IsEnabled = true;
                btnAdd.IsEnabled = true;
                btnRemove.IsEnabled = true;
                table.Rows.Clear();
                grid.ItemsSource = table.AsDataView();
            }
            else
            {
                modified = false;
                train = t;
                title.Content = TryFindResource("pgtrain.look") as string;
                txtID.IsEnabled = false;
                btnModify.Visibility = Visibility.Visible;
                btnSale.Visibility = Visibility.Visible;
                btnSubmit.Visibility = Visibility.Collapsed;
                btnDelete.Visibility = Visibility.Visible;
                grid.IsReadOnly = true;
                txtName.IsEnabled = false;
                stack.IsEnabled = false;
                stack2.IsEnabled = false;
                btnAdd.IsEnabled = false;
                btnRemove.IsEnabled = false;
                SetTrain();
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            if (modified)
            {
                btnModify_Click(null, null);
            }
            parentWindow.GotoTrains(1);
        }

        private void CheckBox1_Checked(object sender, RoutedEventArgs e)
        {
            foreach (CheckBox ch in stack.Children)
            {
                ch.IsChecked = true;
            }
        }

        private void CheckBox2_Checked(object sender, RoutedEventArgs e)
        {
            foreach (CheckBox ch in stack2.Children)
            {
                ch.IsChecked = true;
            }
        }

        private void btnAdd_Click(object sender, RoutedEventArgs e)
        {
            table.Rows.Add();
        }

        private void btnRemove_Click(object sender, RoutedEventArgs e)
        {
            if (grid.SelectedIndex >= 0 && grid.SelectedIndex < table.Rows.Count)
                table.Rows.RemoveAt(grid.SelectedIndex);
        }

        private void CheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            string s = ((CheckBox)sender).Content.ToString();
            if (table.Columns.Contains(s))
                table.Columns.Remove(table.Columns[s]);
            grid.ItemsSource = table.AsDataView();
        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            string s = ((CheckBox)sender).Content.ToString();
            table.Columns.Add(s, typeof(double));
            grid.ItemsSource = table.AsDataView();
        }

        bool modified;
        private void btnModify_Click(object sender, RoutedEventArgs e)
        {
            modified = !modified;
            if (modified)
            {
                btnModify.Content = TryFindResource("pgtrain.btnmodify2") as string;
                btnSale.IsEnabled = false;
                btnDelete.IsEnabled = false;
                grid.IsReadOnly = false;
                txtName.IsEnabled = true;
                stack.IsEnabled = true;
                stack2.IsEnabled = true;
                btnAdd.IsEnabled = true;
                btnRemove.IsEnabled = true;
            }
            else
            {
                btnModify.Content = TryFindResource("pgtrain.btnmodify1") as string;
                btnSale.IsEnabled = true;
                btnDelete.IsEnabled = true;
                grid.IsReadOnly = true;
                txtName.IsEnabled = false;
                stack.IsEnabled = false;
                stack2.IsEnabled = false;
                btnAdd.IsEnabled = false;
                btnRemove.IsEnabled = false;
                if (GetTrain() && clr.ModifyTrain(tmpTrain) == 1)
                {
                    train = tmpTrain;
                    Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrain.modok") as string);
                    dg.ShowDialog();
                }
                else
                {
                    Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrain.moderr") as string);
                    dg.ShowDialog();
                    SetTrain();
                }
            }
        }

        bool GetTrain()
        {
            if (txtID.Text == "" || txtName.Text == "" || txtID.Text.Contains(" ") || txtName.Text.Contains(" "))
                return false;
            tmpTrain = new Train
            {
                trainID = txtID.Text,
                name = txtName.Text
            };
            tmpTrain.catalog = "";
            foreach (RadioButton rd in stack.Children)
            {
                if (rd.IsChecked == true && rd.Content.ToString().Length == 1)
                {
                    tmpTrain.catalog += rd.Content;
                }
            }
            tmpTrain.numPrice = table.Columns.Count - 4;
            tmpTrain.namePrice = new string[12];
            for (int i = 0; i < tmpTrain.numPrice; ++i)
            {
                tmpTrain.namePrice[i] = table.Columns[4 + i].Caption;
            }
            tmpTrain.numStation = table.Rows.Count;
            if (tmpTrain.stations == null)
                tmpTrain.stations = new List<Station>();
            tmpTrain.stations.Clear();
            for (int i = 0; i < tmpTrain.numStation; ++i)
            {
                Station s = new Station();
                s.name = table.Rows[i][0].ToString();
                s.timeArrive = table.Rows[i][1].ToString();
                s.timeStart = table.Rows[i][2].ToString();
                s.timeStopover = table.Rows[i][3].ToString();
                if (s.name == "" || s.timeArrive == "" || s.timeStart == "" || s.timeStopover == "" || s.name.Contains(" ") || s.timeArrive.Contains(" ") || s.timeStart.Contains(" ") || s.timeStopover.Contains(" ") || s.name.Length > 6 || s.timeArrive.Length > 5 || s.timeStart.Length > 5 || s.timeStopover.Length > 5)
                    return false;
                s.price = new double[12];
                for (int j = 0; j < tmpTrain.numPrice; ++j)
                {
                    if (!Double.TryParse(table.Rows[i][4 + j].ToString(), out s.price[j]))
                        return false;
                }
                tmpTrain.stations.Add(s);
            }
            return true;
        }

        private void btnSubmit_Click(object sender, RoutedEventArgs e)
        {
            if (GetTrain() && clr.AddTrain(tmpTrain) == 1)
            {
                train = tmpTrain;
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrain.addok") as string);
                dg.ShowDialog();
                btnBack_Click(null, null);
            }
            else
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrain.adderr") as string);
                dg.ShowDialog();
            }
        }

        private void btnSale_Click(object sender, RoutedEventArgs e)
        {
            if (clr.SaleTrain(train.trainID) == 1)
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrain.saleok") as string);
                dg.ShowDialog();
            }
            else
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrain.saleerr") as string);
                dg.ShowDialog();
            }
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            if (clr.DeleteTrain(train.trainID) == 1)
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrain.deleteok") as string);
                dg.ShowDialog();
                btnBack_Click(null, null);
            }
            else
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgtrain.deleteerr") as string);
                dg.ShowDialog();
            }
        }
    }
}
