#include "ScriptPCH.h"
#include "Vehicle.h"
#include "AchievementMgr.h"
#include "CombatAI.h"


enum Spells
{
    SPELL_FROSTFIRE_BOLT = 44614,
    SPELL_FROST_NOVA = 122,
    SPELL_FIRE_BLAST = 2136,
    SPELL_BLINK = 65793,
    SPELL_SUMMON_WATER_ELEMENTAL = 31687,
};


enum Events
{
    EVENT_FROST_NOVA = 1,
    EVENT_FIRE_BLAST = 2,
    EVENT_BLINK = 3,
    EVENT_ESCAPE_PLAYER = 4,
};

enum eTalks {
    SAY_AGGRO = 0,
    SAY_DEATH = 1,
};

enum eSummons {
    NPC_CONJURED_WATER_ELEMENTAL = 500001,
};


class npc_wandering_mage : public CreatureScript {
public: npc_wandering_mage() : CreatureScript("npc_wandering_mage") {}

      struct npc_wandering_mageAI : public ScriptedAI {

          npc_wandering_mageAI(Creature* creature) : ScriptedAI(creature) {
              me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
              me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
              SetEquipmentSlots(false, 47524, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

              uint32 GlobalCooldown;
              uint32 BuffTimer;
          }

          EventMap events;

          void Reset() override {
              events.Reset();
          }

          void EnterCombat(Unit* who) override {
              events.ScheduleEvent(EVENT_FIRE_BLAST, 5000);
              events.ScheduleEvent(EVENT_ESCAPE_PLAYER, 10000);
          }

          void AttackStart(Unit* who) {
              if (!who)
                  return;

              if (me->Attack(who, true)) {
                  DoStartMovement(who, 20.0f); // Catch up to 20yds to be able to attack
                  SetCombatMovement(true);
              }
          }

          void JustDied(Unit* killer) override {
              Talk(SAY_AGGRO);
              Talk(SAY_DEATH);
          }

          void UpdateAI(uint32 diff) override {
              // This is where we want to stop if there is no victim
              if (!UpdateVictim())
                  return;

              events.Update(diff);

              if (me->HasUnitState(UNIT_STATE_CASTING))
                  return;

              if (uint32 eventId = events.ExecuteEvent()) {
                  switch (eventId) {
                  case EVENT_ESCAPE_PLAYER:
                      events.ScheduleEvent(EVENT_BLINK, 500);
                      events.ScheduleEvent(EVENT_ESCAPE_PLAYER, 15000);
                      break;
                  case EVENT_BLINK:
                      DoCast(SPELL_BLINK);
                      break;
                  case EVENT_FIRE_BLAST:
                      DoCastVictim(SPELL_FIRE_BLAST);
                      events.ScheduleEvent(EVENT_FIRE_BLAST, 5000);
                      break;
                  }
              }

              DoSpellAttackIfReady(SPELL_FROSTFIRE_BOLT);
          }

      };

      CreatureAI* GetAI(Creature* creature) const override {
          return new npc_wandering_mageAI(creature);
      }
};

void AddSC_custom_player_npcs()
{
    new npc_wandering_mage();
}