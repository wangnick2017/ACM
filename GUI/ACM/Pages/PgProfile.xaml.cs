using System;
using System.Collections.Generic;
using System.Globalization;
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
    /// PgProfile.xaml 的交互逻辑
    /// </summary>
    public partial class PgProfile : UserControl
    {
        MainWindow parentWindow;
        ClrBackend clr;
        public PgProfile(MainWindow parent, ClrBackend c)
        {
            InitializeComponent();
            parentWindow = parent;
            clr = c;
        }

        private void BtnModify_Click(object sender, RoutedEventArgs e)
        {
            TxtName_TextChanged(null, null);
            PasswordChanged(txtConfirm, null);
            TxtOld_PasswordChanged(null, null);
            TxtEmail_TextChanged(null, null);
            TxtPhone_TextChanged(null, null);
            TxtValid_TextChanged(null, null);
            if (errConfirm.Visibility == Visibility.Visible || errEmail.Visibility == Visibility.Visible || errName.Visibility == Visibility.Visible || errPhone.Visibility == Visibility.Visible || errValid.Visibility == Visibility.Visible || errOld.Visibility == Visibility.Visible)
                return;
            User u = new User
            {
                ID = parentWindow.user.ID,
                name = txtName.Text,
                password = Something.MD5Encrypt(txtPassword.Password),
                email = txtEmail.Text,
                phone = txtPhone.Text
            };
            if (clr.Login(u.ID, Something.MD5Encrypt(txtOld.Password)) != 1)
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgpro.failed1") as string);
                dg.ShowDialog();
                return;
            }
            if (clr.ModifyProfile(u) == 1)
            {
                Clear();
                parentWindow.user = u;
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgpro.success") as string);
                dg.ShowDialog();
            }
            else
            {
                Dialogs.Dialog dg = new Dialogs.Dialog("", TryFindResource("pgpro.failed2") as string);
                dg.ShowDialog();
            }
            Generate();
        }

        public void SetID(int id)
        {
            labID.Content = "ID：" + id;
            txtName.Text = parentWindow.user.name;
            txtEmail.Text = parentWindow.user.email;
            txtPhone.Text = parentWindow.user.phone;
        }

        void Clear()
        {
            txtPassword.Clear();
            txtConfirm.Clear();
            txtValid.Clear();
            txtOld.Clear();
            errConfirm.Visibility = Visibility.Hidden;
            errEmail.Visibility = Visibility.Hidden;
            errName.Visibility = Visibility.Hidden;
            errPhone.Visibility = Visibility.Hidden;
            errValid.Visibility = Visibility.Hidden;
            errOld.Visibility = Visibility.Hidden;
        }
        bool leave = false;
        public void In()
        {
            leave = false;
        }
        private void BtnBack_Click(object sender, RoutedEventArgs e)
        {
            leave = true;
            Clear();
            passwordChanged = confirmChanged = false;
            parentWindow.GotoUser(-1);
        }

        private void TxtName_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (leave)
                return;
            errName.Visibility = (txtName.Text == "" || txtName.Text.Contains(" ")) ? Visibility.Visible : Visibility.Hidden;
        }

        bool passwordChanged = false, confirmChanged = false;
        private void PasswordChanged(object sender, RoutedEventArgs e)
        {
            if (leave)
                return;
            if (((PasswordBox)sender).Name == "txtConfirm")
                confirmChanged = true;
            else
                passwordChanged = true;
            errConfirm.Visibility = (passwordChanged && txtPassword.Password == "" || confirmChanged && txtConfirm.Password == "" || confirmChanged && txtPassword.Password != txtConfirm.Password || txtPassword.Password.Contains(" ") || txtConfirm.Password.Contains(" ")) ? Visibility.Visible : Visibility.Hidden;
        }

        private void TxtEmail_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (leave)
                return;
            errEmail.Visibility = (txtEmail.Text == "" || txtEmail.Text.Contains(" ") || !Something.EmailString(txtEmail.Text)) ? Visibility.Visible : Visibility.Hidden;
        }

        private void TxtOld_PasswordChanged(object sender, RoutedEventArgs e)
        {
            if (leave)
                return;
            errOld.Visibility = (txtOld.Password == "" || txtOld.Password.Contains(" ")) ? Visibility.Visible : Visibility.Hidden;
        }

        private void TxtValid_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (leave)
                return;
            errValid.Visibility = (txtValid.Text != code) ? Visibility.Visible : Visibility.Hidden;
        }

        private void TxtPhone_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (leave)
                return;
            errPhone.Visibility = (txtPhone.Text == "" || txtPhone.Text.Contains(" ") || !Int64.TryParse(txtPhone.Text, out long ret)) ? Visibility.Visible : Visibility.Hidden;
        }

        private void Img_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Generate();
        }

        string code;
        public void Generate()
        {
            Random random = new Random();
            code = Convert.ToString(random.Next(0, 9)) + Convert.ToString((char)('A' + random.Next(0, 25))) + Convert.ToString(random.Next(0, 9)) + Convert.ToString((char)('a' + random.Next(0, 25)));
            for (int i = 0; i < code.Length; i++)
            {
                DrawingVisual drawingVisual = new DrawingVisual();
                DrawingContext drawingContext = drawingVisual.RenderOpen();

                drawingContext.DrawText(
                    new FormattedText(Convert.ToString(code[0]), CultureInfo.GetCultureInfo("en-us"),
                        FlowDirection.LeftToRight, new Typeface("Verdana"), random.Next(15, 25), Brushes.Green),
                    new Point(15, 8));

                drawingContext.DrawText(
                    new FormattedText(Convert.ToString(code[1]), CultureInfo.GetCultureInfo("en-us"),
                        FlowDirection.LeftToRight, new Typeface("Cambria"), random.Next(15, 25), Brushes.Blue),
                    new Point(30, 8));

                drawingContext.DrawText(
                    new FormattedText(Convert.ToString(code[2]), CultureInfo.GetCultureInfo("en-us"),
                        FlowDirection.LeftToRight, new Typeface("Verdana"), random.Next(15, 25), Brushes.Black),
                    new Point(45, 8));

                drawingContext.DrawText(
                    new FormattedText(Convert.ToString(code[3]), CultureInfo.GetCultureInfo("en-us"),
                        FlowDirection.LeftToRight, new Typeface("Consolas"), random.Next(15, 25), Brushes.DeepSkyBlue),
                    new Point(60, 8));

                drawingContext.DrawLine(new Pen(Brushes.Purple, 1),
                    new Point(random.NextDouble() * img.Width, random.NextDouble() * img.Height),
                    new Point(random.NextDouble() * img.Width, random.NextDouble() * img.Height));
                drawingContext.DrawLine(new Pen(Brushes.Green, 1),
                    new Point(random.NextDouble() * img.Width, random.NextDouble() * img.Height),
                    new Point(random.NextDouble() * img.Width, random.NextDouble() * img.Height));
                drawingContext.DrawLine(new Pen(Brushes.DarkGray, 1),
                    new Point(random.NextDouble() * img.Width, random.NextDouble() * img.Height),
                    new Point(random.NextDouble() * img.Width, random.NextDouble() * img.Height));

                drawingContext.Close();

                RenderTargetBitmap renderBitmap = new RenderTargetBitmap((int)this.img.Width, (int)this.img.Height, 96, 96, PixelFormats.Pbgra32);
                renderBitmap.Render(drawingVisual);
                ImageSource source = BitmapFrame.Create(renderBitmap);
                img.Source = source;
            }
        }
    }
}
