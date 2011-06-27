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
using System.Windows.Shell;

namespace Builder.View
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            try
            {
                InitializeComponent();
                DataContext = new MainWindowModel();
            }
            catch (Exception e)
            {
                MessageBox.Show("Failed to start: " + e.Message);
            }
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

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            ContentItem item = (ContentItem)((MenuItem)sender).DataContext;
            item.Validate();
            item.Build();
        }

        private void MenuItem_Click_1(object sender, RoutedEventArgs e)
        {
            ContentItem item = (ContentItem)((MenuItem)sender).DataContext;
            item.Clean();
            item.Validate();
        }

        private void ProgressBar_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            taskbarItem.ProgressState = e.NewValue > 0.0f ? TaskbarItemProgressState.Normal : TaskbarItemProgressState.None;
            taskbarItem.ProgressValue = e.NewValue / 100.0f;
        }

        private void MenuItem_Click_2(object sender, RoutedEventArgs e)
        {
            ((MainWindowModel)DataContext).Builder.OutputText.Clear();
        }
    }
}
