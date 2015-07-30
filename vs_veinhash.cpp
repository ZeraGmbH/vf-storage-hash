#include "vs_veinhash.h"

#include <vcmp_componentdata.h>
#include <vcmp_entitydata.h>
#include <vcmp_errordata.h>
#include <ve_commandevent.h>
#include <QEvent>

Q_LOGGING_CATEGORY(VEIN_STORAGE_HASH, "\033[1;36m<Vein.Storage.Hash>\033[0m")
Q_LOGGING_CATEGORY(VEIN_STORAGE_HASH_VERBOSE, "\033[0;36m<Vein.Storage.Hash>\033[0m")


using namespace VeinEvent;
using namespace VeinComponent;

namespace VeinStorage
{
  VeinHash::VeinHash(QObject *t_parent) :
    StorageSystem(t_parent)
  {
    qCDebug(VEIN_STORAGE_HASH) << "Created VeinHash storage";
  }

  VeinHash::~VeinHash()
  {
    qCDebug(VEIN_STORAGE_HASH) << "Destroyed VeinHash storage";
    for(int i=0; i<m_data->count(); ++i)
    {
      QHash<QString, QVariant> *tmpToDelete = m_data->values().at(i);
      delete tmpToDelete;
    }
    delete m_data;
  }


  bool VeinHash::processEvent(QEvent *t_event)
  {
    bool retVal = false;

    if(t_event->type()==CommandEvent::eventType())
    {
      CommandEvent *cEvent = 0;
      cEvent = static_cast<CommandEvent *>(t_event);

      if(cEvent != 0 && cEvent->eventSubtype() == CommandEvent::EventSubtype::NOTIFICATION)
      {
        switch (cEvent->eventData()->type())
        {
          case ComponentData::dataType():
          {
            ComponentData *cData=0;
            cData = static_cast<ComponentData *>(cEvent->eventData());
            qCDebug(VEIN_STORAGE_HASH_VERBOSE) << "Processing component data from event" << cEvent;
            retVal = processComponentData(cData);
            break;
          }
          case EntityData::dataType():
          {
            EntityData *eData=0;
            eData = static_cast<EntityData *>(cEvent->eventData());
            qCDebug(VEIN_STORAGE_HASH_VERBOSE) << "Processing entity data from event" << cEvent;
            retVal = processEntityEvent(eData);
            break;
          }
          default:
            break;
        }
      }
    }

    return retVal;
  }

  StorageSystem::StorageType VeinHash::getStorageType() const
  {
    return StorageSystem::MEMORY_STORAGE;
  }

  void VeinHash::dumpToFile(QIODevice *t_fileDevice, bool t_overwrite) const
  {
    /** @todo TBD */
    Q_UNUSED(t_fileDevice)
    Q_UNUSED(t_overwrite)
    qCWarning(VEIN_STORAGE_HASH) << "Function dumpToFile is currently not implemented";
  }


  QVariant VeinHash::getStoredValue(int t_entityId, const QString &t_componentName) const
  {
    QVariant retVal;
    if(m_data->contains(t_entityId))
    {
      retVal = m_data->value(t_entityId)->value(t_componentName);
    }
    else
    {
      qCWarning(VEIN_STORAGE_HASH) << "Unknown entity with id:" <<  t_entityId << "component" << t_componentName;
    }
    return retVal;
  }

  bool VeinHash::hasStoredValue(int t_entityId, const QString &t_componentName) const
  {
    if(m_data->contains(t_entityId))
    {
      return m_data->value(t_entityId)->contains(t_componentName);
    }
    else
    {
      return false;
    }
  }


  bool VeinHash::initializeData(const QUrl &t_sourceUrl)
  {
    Q_UNUSED(t_sourceUrl)
    qCWarning(VEIN_STORAGE_HASH) << "Function initializeData is currently not implemented";
    return false;
  }

  const QHash<QString, QVariant> *VeinHash::getEntityDataCopy(int t_entityId) const
  {
    return m_data->value(t_entityId);
  }

  bool VeinHash::hasEntity(int t_entityId) const
  {
    return m_data->contains(t_entityId);
  }

  QList<int> VeinHash::getEntityList() const
  {
    return m_data->keys();
  }

  bool VeinHash::processComponentData(ComponentData *t_cData)
  {
    bool retVal=false;
    QString  componentName= t_cData->componentName();
    switch(t_cData->eventCommand())
    {
      case ComponentData::Command::CCMD_ADD:
      {
        if(m_data->contains(t_cData->entityId()) == false)
        {
          QString tmpErrorString = tr("can not add value for invalid entity id: %1").arg(t_cData->entityId());
          qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
          sendError(tmpErrorString, t_cData);
        }
        else if(m_data->value(t_cData->entityId())->contains(componentName) == true)
        {
          QString tmpErrorString = tr("value already exists for component: %1 %2").arg(t_cData->entityId()).arg(t_cData->componentName());
          qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
          sendError(tmpErrorString, t_cData);
        }
        else
        {
          qCDebug(VEIN_STORAGE_HASH) << "adding component:" << t_cData->entityId() << componentName << "with value:" << t_cData->newValue();
          m_data->value(t_cData->entityId())->insert(componentName,t_cData->newValue());
          retVal = true;
        }
        break;
      }

      case ComponentData::Command::CCMD_REMOVE:
      {
        qCDebug(VEIN_STORAGE_HASH) << "removing key:" << componentName;
        m_data->value(t_cData->entityId())->remove(componentName);
        retVal = true;
        break;
      }

      case ComponentData::Command::CCMD_SET:
      {
        if(m_data->contains(t_cData->entityId()) == false)
        {
          QString tmpErrorString = tr("can not set value for nonexistant entity id: %1").arg(t_cData->entityId());
          qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
          sendError(tmpErrorString, t_cData);
        }
        else if(m_data->value(t_cData->entityId())->contains(componentName) == false)
        {
          QString tmpErrorString = tr("can not set value for nonexistant component: %1 %2").arg(t_cData->entityId()).arg(t_cData->componentName());
          qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
          sendError(tmpErrorString, t_cData);
        }
        else
        {
          qCDebug(VEIN_STORAGE_HASH_VERBOSE) << "setting key:" << componentName << "from:" << m_data->value(t_cData->entityId())->value(componentName) << "to:" << t_cData->newValue();
          m_data->value(t_cData->entityId())->insert(componentName,t_cData->newValue());
          retVal = true;
        }
        break;
      }

      default:
        break;
    }
    return retVal;
  }

  bool VeinHash::processEntityEvent(EntityData *t_eData)
  {
    bool retVal =false;
    switch(t_eData->eventCommand())
    {
      case VeinComponent::EntityData::ECMD_ADD:
      {
        if(m_data->contains(t_eData->entityId()) == false)
        {
          QHash<QString, QVariant> *tmpHash = new QHash<QString, QVariant>();
          m_data->insert(t_eData->entityId(), tmpHash);
          retVal = true;
        }
        else
        {
          QString tmpErrorString = tr("Cannot add entity, entity id already exists: %1").arg(t_eData->entityId());
          qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
          sendError(tmpErrorString, t_eData);
        }
        break;
      }

      case VeinComponent::EntityData::ECMD_REMOVE:
      {
        if(m_data->contains(t_eData->entityId()) == true)
        {
          QHash<QString, QVariant> *tmpHash = m_data->value(t_eData->entityId());
          m_data->remove(t_eData->entityId());
          delete tmpHash;
          retVal = true;
        }
        else
        {
          QString tmpErrorString = tr("Cannot delete entity, entity id does not exists: %1").arg(t_eData->entityId());
          qCWarning(VEIN_STORAGE_HASH) << tmpErrorString;
          sendError(tmpErrorString, t_eData);
        }
        break;
      }

      default: //ECMD_SUBSCRIBE etc. is handled by the networksystem
        break;
    }
    return retVal;
  }

  void VeinHash::sendError(const QString t_errorString, EventData *t_data)
  {
    ErrorData *errData = new ErrorData();

    errData->setEntityId(t_data->entityId());
    errData->setOriginalData(t_data);
    errData->setEventOrigin(EventData::EventOrigin::EO_LOCAL);
    errData->setEventTarget(t_data->eventTarget());
    errData->setErrorDescription(t_errorString);

    CommandEvent *cEvent = new CommandEvent(CommandEvent::EventSubtype::NOTIFICATION, errData);
    emit sigSendEvent(cEvent);
  }
}
