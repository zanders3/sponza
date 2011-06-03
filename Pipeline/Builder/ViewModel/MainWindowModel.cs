using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Builder.ViewModel;

namespace Builder.Model
{
    public class MainWindowModel
    {
        public MainWindowModel()
        {
        }

        public Builder Builder
        {
            get { return Builder.Instance; }
        }

        public Command Build
        {
            get { return null; }
        }
    }
}
