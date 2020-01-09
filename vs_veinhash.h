#ifndef VEINHASH_H
#define VEINHASH_H

#include "globalIncludes.h"

#include <ve_storagesystem.h>
#include <ve_eventdata.h>
#include <QHash>

Q_DECLARE_LOGGING_CATEGORY(VEIN_STORAGE_HASH)
Q_DECLARE_LOGGING_CATEGORY(VEIN_STORAGE_HASH_VERBOSE)

namespace VeinEvent
{
  class CommandEvent;
  class EventData;
}

namespace VeinComponent
{
  class ComponentData;
  class EntityData;
}

/**
 * @brief Namespace for Vein Framework storage implementations
 */
namespace VeinStorage
{
  /**
   * @brief A QHash based VeinEvent::StorageSystem implementation
   */
  class VFSTORAGEHASH_EXPORT VeinHash : public VeinEvent::StorageSystem
  {
    Q_OBJECT

  public:
    explicit VeinHash(QObject *t_parent=nullptr);
    void setAcceptableOrigin(QList<VeinEvent::EventData::EventOrigin> t_origins);
    const QList<VeinEvent::EventData::EventOrigin> &getAcceptableOrigin() const;

    virtual ~VeinHash() override;

    //stands for QHash<"entity descriptor", QHash<"component name", "component data">*>
    template <typename T>
    using ComponentStorage = QHash<T, QHash<QString, QVariant>*>;

    //VeinEvent::StorageSystem interface
  public:
    bool processEvent(QEvent *t_event) override;
    StorageType getStorageType() const override;
    void dumpToFile(QFile *t_fileDevice, bool t_overwrite) const  override;
    Q_INVOKABLE QVariant getStoredValue(int t_entityId, const QString &t_componentName) const override;
    Q_INVOKABLE bool hasStoredValue(int t_entityId, const QString &t_componentName) const override;
    bool initializeData(const QUrl &t_sourceUrl) override;
    QList<QString> getEntityComponents(int t_entityId) const override;
    bool hasEntity(int t_entityId) const override;
    QList<int> getEntityList() const override;
  private:
    /**
     * @brief handles ADD, REMOVE and SET for ComponentData events
     * @param t_cData
     * @return
     */
    bool processComponentData(VeinComponent::ComponentData *t_cData);

    /**
     * @brief handles ADD and REMOVE for EntityData events
     * @param t_eData
     * @return
     */
    bool processEntityData(VeinComponent::EntityData *t_eData);

    /**
     * @brief sends ErrorEvents with the EventDate that caused the error
     * @param t_errorString
     * @param t_data
     */
    void sendError(const QString &t_errorString, VeinEvent::EventData *t_data);

    ComponentStorage<int> *m_data = new ComponentStorage<int>();
    QList<VeinEvent::EventData::EventOrigin> m_acceptableOrigins = {VeinEvent::EventData::EventOrigin::EO_LOCAL, VeinEvent::EventData::EventOrigin::EO_FOREIGN};
  };
}

#endif // VEINHASH_H
