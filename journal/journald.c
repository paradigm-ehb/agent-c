#include <assert.h>
#include <systemd/sd-journal.h>
#include "base/base.h"
#include "journald.h"

global_variable const char *journal_field_names[JOURNAL_FIELD_COUNT] = {
  [JOURNAL_FIELD_PRIORITY] = "PRIORITY",
  [JOURNAL_FIELD_MESSAGE] = "MESSAGE",
  [JOURNAL_FIELD_TIMESTAMP] = "__REALTIME_TIMESTAMP",
  [JOURNAL_FIELD_PID] = "_PID",
  [JOURNAL_FIELD_UID] = "_UID",
  [JOURNAL_FIELD_GID] = "_GID",
  [JOURNAL_FIELD_COMM] = "_COMM",
  [JOURNAL_FIELD_EXE] = "_EXE",
  [JOURNAL_FIELD_SYSTEMD_UNIT] = "_SYSTEMD_UNIT",
  [JOURNAL_FIELD_SYSTEMD_USER_UNIT] = "_SYSTEMD_USER_UNIT",
  [JOURNAL_FIELD_BOOT_ID] = "_BOOT_ID",
  [JOURNAL_FIELD_MACHINE_ID] = "_MACHINE_ID",
  [JOURNAL_FIELD_HOSTNAME] = "_HOSTNAME",
  [JOURNAL_FIELD_TRANSPORT] = "_TRANSPORT",
};

internal JournalField
parse_unit_name(char *unit, JournalField jf)
{
  return JOURNAL_FIELD_HOSTNAME;
}

internal const char *
logs(char *unit, char *time)
{
  sd_journal *journal = NULL;

  int code = sd_journal_open(&journal, SD_JOURNAL_LOCAL_ONLY);
  if (code != 0)
  {
    assert(0);
  }

  sd_journal_seek_head(journal);

  const char *msg = { 0 };
  while ((code = sd_journal_next(journal)) > 0)
  {
    {
      const void *data;

      sd_journal_add_match(journal, &data, sizeof((char *)jf));
    }
    {
      const void *data;
      size_t len = 0;

      if (sd_journal_get_data(journal, "MESSAGE", &data, &len) == 0)
      {
        msg = (const char *)data;
      }
    }
  }

  sd_journal_close(journal);

  return msg;
}
