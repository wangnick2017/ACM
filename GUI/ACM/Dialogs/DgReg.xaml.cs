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

namespace ACM.Dialogs
{
    /// <summary>
    /// Dialog.xaml 的交互逻辑
    /// </summary>
    public partial class DgReg : Window
    {
        public DgReg(int id)
        {
            InitializeComponent();
            txt.Text = TryFindResource("dgreg.suc1") as string + id + TryFindResource("dgreg.suc2") as string;
            Return = 1;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            InitializeControl();
        }

        Button btnClose;

        void InitializeControl()
        {
            if (Application.Current.FindResource("ACMDialogControlTemplate") is ControlTemplate template)
            {
                Label moveableLabel = template.FindName("MoveableLabel", this) as Label;
                moveableLabel.MouseMove += MoveableLabel_MouseMove;
                btnClose = template.FindName("CloseButton", this) as Button;
                btnClose.Click += BtnClose_Click;
            }
        }

        void MoveableLabel_MouseMove(object sender, MouseEventArgs e)
        {
            if (Mouse.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void BtnClose_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        public int Return;

        private void Button1_Click(object sender, RoutedEventArgs e)
        {
            Return = 0;
            Close();
        }

        private void Button2_Click(object sender, RoutedEventArgs e)
        {
            Return = 1;
            Close();
        }
    }
}
