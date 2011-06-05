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
using Builder.Model;

namespace Builder.View
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            DataContext = new MainWindowModel();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            ((MainWindowModel)DataContext).Builder.Dispose();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ((MainWindowModel)DataContext).Builder.Build();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            ((MainWindowModel)DataContext).Builder.Clean();
        }
    }
}
