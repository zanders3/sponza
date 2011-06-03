using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;

namespace Builder.ViewModel
{
    public class Command : ICommand
    {
        private Action<object> m_execute;

        public Command(Action<object> execute)
        {
            m_execute = execute;
        }

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public event EventHandler CanExecuteChanged;

        public void Execute(object parameter)
        {
            m_execute(parameter);
        }
    }
}
