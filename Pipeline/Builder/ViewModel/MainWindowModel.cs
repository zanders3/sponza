using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Builder.ViewModel;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.ComponentModel;

namespace Builder.Model
{
    [ValueConversion(typeof(ContentItem.State), typeof(Brush))]
    public class ContentStateColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            switch ((ContentItem.State)value)
            {
                case ContentItem.State.Building:
                    return Brushes.Gold;
                case ContentItem.State.Failed:
                    return Brushes.Red;
                case ContentItem.State.Succeeded:
                default:
                    return Brushes.YellowGreen;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class MainWindowModel : BasePropertyChanged
    {
        public MainWindowModel()
        {
            Builder = new Builder();
        }

        public Builder Builder
        {
            get;
            private set;
        }

        public Command BuildItem
        {
            get
            {
                return new Command(item =>
                    {
                    });
            }
        }

        public Command CleanItem
        {
            get
            {
                return new Command(item =>
                {
                });
            }
        }
    }
}
