using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.IO;

namespace Builder.Model.XML
{
    public class BuilderXML
    {
        public BuilderXML()
        {
        }

        public BuilderXML(IEnumerable<ContentItem> items)
        {
            ContentItems = items.Select(dep => new ContentItemXML(dep)).ToList();
        }

        public List<ContentItemXML> ContentItems;

        private static XmlSerializer m_formatter = new XmlSerializer(typeof(BuilderXML));

        public void Save(string file)
        {
            using (Stream stream = File.Create(file))
            {
                m_formatter.Serialize(stream, this);
            }
        }

        public static BuilderXML Load(string file)
        {
            using (Stream stream = File.Open(file, FileMode.Open))
            {
                return (BuilderXML)m_formatter.Deserialize(stream);
            }
        }
    }

    public class ContentItemXML
    {
        [XmlAttribute]
        public string Resource;
        [XmlAttribute]
        public string Output;
        
        [XmlArrayItem]
        public List<ContentItemXML> Dependencies;

        public ContentItemXML()
        {
        }

        public ContentItemXML(ContentItem item)
        {
            Resource = item.ResourcePath;
            Output = item.OutputPath;
            Dependencies = item.Dependencies.Select(dep => new ContentItemXML(dep)).ToList();
        }

        public ContentItem ToContentItem()
        {
            return new ContentItem(Resource, Output, Dependencies.Select(dep => dep.ToContentItem()));
        }
    }
}
