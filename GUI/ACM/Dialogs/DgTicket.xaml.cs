using System;
using System.Collections.Generic;
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
using System.Windows.Shapes;
using Clr;

namespace ACM.Dialogs
{
    /// <summary>
    /// Dialog.xaml 的交互逻辑
    /// </summary>
    public partial class DgTicket : Window
    {
        Ticket ticket;
        ListItem parent;
        int mode;
        public DgTicket(Ticket t, ListItem p)
        {
            InitializeComponent();
            parent = p;
            mode = p.Mode;
            ticket = t;
            ID.Content = ticket.trainID;
            locFrom.Content = ticket.locFrom;
            locTo.Content = ticket.locTo;
            timeFrom.Content = ticket.dateFrom + " " + ticket.timeFrom;
            timeTo.Content = ticket.dateTo + " " + ticket.timeTo;
            for (int i = 0; i < ticket.cnt; ++i)
                com.Items.Add(ticket.ticketKind[i] + "  " + (p.Mode == 0 ? TryFindResource("ticket.left") as string : TryFindResource("ticket.booked") as string) + " " + ticket.num[i].ToString() + " " + TryFindResource("ticket.ticket") as string + "  ¥" + ticket.price[i].ToString());
            com.SelectedIndex = 0;
            com2.SelectedIndex = p.Mode;
        }

        private void Label_MouseUp(object sender, MouseButtonEventArgs e)
        {
            Close();
        }

        private void Label_MouseMove(object sender, MouseEventArgs e)
        {
            if (Mouse.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            err.Visibility = Visibility.Hidden;
            if (com.SelectedIndex < 0 || com.SelectedIndex >= com.Items.Count ||
                com2.SelectedIndex < 0 || com2.SelectedIndex > 1 || !Int32.TryParse(txtCnt.Text, out int cnt))
            {
                err.Visibility = Visibility.Visible;
                return;
            }
            if (!parent.ChangeTicket(com.SelectedIndex, cnt))
                err.Visibility = Visibility.Visible;
            else
            {
                int si = com.SelectedIndex;
                string s = com.SelectedItem.ToString();
                for (int i = 0; i < ticket.cnt; ++i)
                    if (s.Contains(ticket.ticketKind[i]))
                    {
                        ticket.num[i] -= cnt;
                        com.Items[si] = ticket.ticketKind[i] + "  " + (mode == 0 ? TryFindResource("ticket.left") as string : TryFindResource("ticket.booked") as string) + " " + (ticket.num[i]).ToString() + " " + TryFindResource("ticket.ticket") as string + "  ¥" + ticket.price[i].ToString();
                        com.SelectedIndex = si;
                    }
                parent.Reset();
            }
        }

        private void Com_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            err.Visibility = Visibility.Hidden;
        }

    }
}
