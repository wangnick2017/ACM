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
using System.Windows.Navigation;
using System.Windows.Shapes;
using Clr;

namespace ACM
{
    /// <summary>
    /// ListItem.xaml 的交互逻辑
    /// </summary>
    public partial class ListItem : ListBoxItem
    {
        Ticket ticket;
        ClrBackend clr;
        StackPanel[] stks = new StackPanel[10];
        int userID;
        public int Mode;
        public ListItem()
        {
            InitializeComponent();
        }
        public ListItem(Ticket t, ClrBackend c, int id, int m)
        {
            InitializeComponent();
            Mode = m;
            clr = c;
            userID = id;
            stks[0] = stk1;
            stks[1] = stk2;
            stks[2] = stk3;
            stks[3] = stk4;
            stks[4] = stk5;
            stks[5] = stk6;
            stks[6] = stk7;
            stks[7] = stk8;
            stks[8] = stk9;
            stks[9] = stk10;
            ticket = t;
            Reset();
        }

        public void Reset()
        {
            ID.Text = ticket.trainID;
            locFrom.Text = ticket.locFrom;
            locTo.Text = ticket.locTo;
            timeFrom.Text = ticket.dateFrom + " " + ticket.timeFrom;
            timeTo.Text = ticket.dateTo + " " + ticket.timeTo;
            for (int i = 0; i < ticket.cnt; ++i)
            {
                foreach (TextBlock u in stks[i].Children)
                {
                    if (u.Name.Contains("nm"))
                        u.Text = ticket.ticketKind[i];
                    else
                        u.Text = ticket.num[i].ToString() + " " + (TryFindResource("ticket.ticket") as string) + "\n¥" + ticket.price[i].ToString();
                }
                stks[i].Visibility = Visibility.Visible;
            }
            for (int i = ticket.cnt; i < 10; ++i)
                stks[i].Visibility = Visibility.Collapsed;
        }

        public bool ChangeTicket(int th, int cnt)
        {
            if (Mode == 0)
                return clr.BuyTicket(userID, cnt, ticket.trainID, ticket.locFrom, ticket.locTo, ticket.dateFrom, ticket.ticketKind[th]) == 1;
            else
                return clr.RefundTicket(userID, cnt, ticket.trainID, ticket.locFrom, ticket.locTo, ticket.dateFrom, ticket.ticketKind[th]) == 1;
        }

        private void ListBoxItem_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Dialogs.DgTicket dg = new Dialogs.DgTicket(ticket, this);
            dg.ShowDialog();
        }
    }
}
