#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#if __STDC_VERSION__ >= 201112L && !defined (__STDC_NO_THREADS__) && defined (CLAP_HAS_THREADS_H)
#   define CLAP_HAS_THREAD
#   include <threads.h>
#endif

#include <clap/clap.h>

static const clap_plugin_descriptor_t s_echo_plugin_desc = {
  .clap_version = CLAP_VERSION_INIT,
  .id = "org.major.EchoPlugin",
  .name = "Echo plugin",
  .vendor = "major soft",
  .url = "https://www.github.com/majorx234/clap_plugin_example",
  .manual_url = "https://www.github.com/majorx234/clap_plugin_example/docs",
  .support_url = "https://www.github.com/majorx234/clap_plugin_example",
  .version = "0.1.0",
  .description = "a simple echo plugin",
  .features = (const char *[]){CLAP_PLUGIN_FEATURE_INSTRUMENT,
    CLAP_PLUGIN_FEATURE_STEREO, NULL},
};

typedef struct {
  clap_plugin_t                   plugin;
  const clap_host_t              *host;
  const clap_host_latency_t      *host_latency;
  const clap_host_log_t          *host_log;
  const clap_host_thread_check_t *host_thread_check;
  const clap_host_state_t        *host_state;

  uint32_t latency;
} echo_plugin_t;

/////////////////////////////
// clap_plugin_audio_ports //
/////////////////////////////
static uint32_t echo_plugin_audio_ports_count(const clap_plugin_t *plugin, bool is_input) {
  // We just declare 1 audio input and 1 audio output
  return 1;
}

static bool echo_plugin_audio_ports_get(const clap_plugin_t    *plugin,
                                        uint32_t                index,
                                        bool                    is_input,
                                        clap_audio_port_info_t *info) {
  if (index > 0)
    return false;
  info->id = 0;
  snprintf(info->name, sizeof(info->name), "%s", "EchoPluginPort");
  info->channel_count = 2;
  info->flags = CLAP_AUDIO_PORT_IS_MAIN;
  info->port_type = CLAP_PORT_STEREO;
  info->in_place_pair = CLAP_INVALID_ID;
  return true;
}

static const clap_plugin_audio_ports_t s_echo_plugin_audio_ports = {
  .count = echo_plugin_audio_ports_count,
  .get = echo_plugin_audio_ports_get,
};

////////////////////////////
// clap_plugin_note_ports //
////////////////////////////

static uint32_t echo_plugin_note_ports_count(const clap_plugin_t *plugin, bool is_input) {
  // We just declare 1 note input
  return 1;
}

static bool echo_plugin_note_ports_get(const clap_plugin_t   *plugin,
                                       uint32_t               index,
                                       bool                   is_input,
                                       clap_note_port_info_t *info) {
  if (index > 0)
    return false;
  info->id = 0;
  snprintf(info->name, sizeof(info->name), "%s", "EchoPluginNotePort");
  info->supported_dialects =
      CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2;
  info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
  return true;
}

static const clap_plugin_note_ports_t s_echo_plugin_note_ports = {
  .count = echo_plugin_note_ports_count,
  .get = echo_plugin_note_ports_get,
};

//////////////////
// clap_latency //
//////////////////

uint32_t echo_plugin_latency_get(const clap_plugin_t *plugin) {
  echo_plugin_t *plug = plugin->plugin_data;
  return plug->latency;
}

static const clap_plugin_latency_t s_echo_plugin_latency = {
  .get = echo_plugin_latency_get,
};

////////////////
// clap_state //
////////////////

bool echo_plugin_state_save(const clap_plugin_t *plugin, const clap_ostream_t *stream) {
   echo_plugin_t *plug = plugin->plugin_data;
   // TODO: write the state into stream
   return true;
}

bool echo_plugin_state_load(const clap_plugin_t *plugin, const clap_istream_t *stream) {
   echo_plugin_t *plug = plugin->plugin_data;
   // TODO: read the state from stream
   return true;
}

static const clap_plugin_state_t s_echo_plugin_state = {
   .save = echo_plugin_state_save,
   .load = echo_plugin_state_load,
};

/////////////////
// clap_plugin //
/////////////////

static bool echo_plugin_init(const struct clap_plugin *plugin) {
  echo_plugin_t *plug = plugin->plugin_data;

  // Fetch host's extensions here
  // Make sure to check that the interface functions are not null pointers
  plug->host_log = (const clap_host_log_t *)plug->host->get_extension(plug->host, CLAP_EXT_LOG);
  plug->host_thread_check = (const clap_host_thread_check_t *)plug->host->get_extension(plug->host, CLAP_EXT_THREAD_CHECK);
  plug->host_latency = (const clap_host_latency_t *)plug->host->get_extension(plug->host, CLAP_EXT_LATENCY);
  plug->host_state = (const clap_host_state_t *)plug->host->get_extension(plug->host, CLAP_EXT_STATE);
  return true;
}

static void echo_plugin_destroy(const struct clap_plugin *plugin) {
  echo_plugin_t *plug = plugin->plugin_data;
  free(plug);
}

static bool echo_plugin_activate(const struct clap_plugin *plugin,
                                 double                    sample_rate,
                                 uint32_t                  min_frames_count,
                                 uint32_t                  max_frames_count) {
  return true;
}

static void echo_plugin_deactivate(const struct clap_plugin *plugin) {}

static bool echo_plugin_start_processing(const struct clap_plugin *plugin) { return true; }

static void echo_plugin_stop_processing(const struct clap_plugin *plugin) {}

static void echo_plugin_reset(const struct clap_plugin *plugin) {}

static void echo_plugin_process_event(echo_plugin_t *plug, const clap_event_header_t *hdr) {
  if (hdr->space_id == CLAP_CORE_EVENT_SPACE_ID) {
    switch (hdr->type) {
      case CLAP_EVENT_NOTE_ON: {
        const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
        // TODO: handle note on
        break;
      }

      case CLAP_EVENT_NOTE_OFF: {
        const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
        // TODO: handle note off
        break;
      }

      case CLAP_EVENT_NOTE_CHOKE: {
        const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
        // TODO: handle note choke
        break;
      }

      case CLAP_EVENT_NOTE_EXPRESSION: {
        const clap_event_note_expression_t *ev = (const clap_event_note_expression_t *)hdr;
        // TODO: handle note expression
        break;
      }

      case CLAP_EVENT_PARAM_VALUE: {
        const clap_event_param_value_t *ev = (const clap_event_param_value_t *)hdr;
        // TODO: handle parameter change
        break;
      }

      case CLAP_EVENT_PARAM_MOD: {
        const clap_event_param_mod_t *ev = (const clap_event_param_mod_t *)hdr;
        // TODO: handle parameter modulation
        break;
      }

      case CLAP_EVENT_TRANSPORT: {
        const clap_event_transport_t *ev = (const clap_event_transport_t *)hdr;
        // TODO: handle transport event
        break;
      }

      case CLAP_EVENT_MIDI: {
        const clap_event_midi_t *ev = (const clap_event_midi_t *)hdr;
        // TODO: handle MIDI event
        break;
      }

      case CLAP_EVENT_MIDI_SYSEX: {
        const clap_event_midi_sysex_t *ev = (const clap_event_midi_sysex_t *)hdr;
        // TODO: handle MIDI Sysex event
        break;
      }

      case CLAP_EVENT_MIDI2: {
        const clap_event_midi2_t *ev = (const clap_event_midi2_t *)hdr;
        // TODO: handle MIDI2 event
        break;
      }
    }
  }
}

static clap_process_status echo_plugin_process(const struct clap_plugin *plugin,
                                               const clap_process_t     *process) {
  echo_plugin_t     *plug = plugin->plugin_data;
  const uint32_t nframes = process->frames_count;
  const uint32_t nev = process->in_events->size(process->in_events);
  uint32_t       ev_index = 0;
  uint32_t       next_ev_frame = nev > 0 ? 0 : nframes;

  for (uint32_t i = 0; i < nframes;) {
    /* handle every events that happrens at the frame "i" */
    while (ev_index < nev && next_ev_frame == i) {
      const clap_event_header_t *hdr = process->in_events->get(process->in_events, ev_index);
      if (hdr->time != i) {
        next_ev_frame = hdr->time;
        break;
      }

      echo_plugin_process_event(plug, hdr);
      ++ev_index;

      if (ev_index == nev) {
        // we reached the end of the event list
        next_ev_frame = nframes;
        break;
      }
    }

    /* process every samples until the next event */
    for (; i < next_ev_frame; ++i) {
      // fetch input samples
      const float in_l = process->audio_inputs[0].data32[0][i];
      const float in_r = process->audio_inputs[0].data32[1][i];

      /* TODO: process samples, here we simply swap left and right channels */
      const float out_l = in_r;
      const float out_r = in_l;

      // store output samples
      process->audio_outputs[0].data32[0][i] = out_l;
      process->audio_outputs[0].data32[1][i] = out_r;
    }
  }

  return CLAP_PROCESS_CONTINUE;
}

static const void *echo_plugin_get_extension(const struct clap_plugin *plugin, const char *id) {
  if (!strcmp(id, CLAP_EXT_LATENCY))
    return &s_echo_plugin_latency;
  if (!strcmp(id, CLAP_EXT_AUDIO_PORTS))
    return &s_echo_plugin_audio_ports;
  if (!strcmp(id, CLAP_EXT_NOTE_PORTS))
    return &s_echo_plugin_note_ports;
  if (!strcmp(id, CLAP_EXT_STATE))
    return &s_echo_plugin_state;
  // TODO: add support to CLAP_EXT_PARAMS
  return NULL;
}

static void echo_plugin_on_main_thread(const struct clap_plugin *plugin) {}

clap_plugin_t *echo_plugin_create(const clap_host_t *host) {
  echo_plugin_t *p = calloc(1, sizeof(*p));
  p->host = host;
  p->plugin.desc = &s_echo_plugin_desc;
  p->plugin.plugin_data = p;
  p->plugin.init = echo_plugin_init;
  p->plugin.destroy = echo_plugin_destroy;
  p->plugin.activate = echo_plugin_activate;
  p->plugin.deactivate = echo_plugin_deactivate;
  p->plugin.start_processing = echo_plugin_start_processing;
  p->plugin.stop_processing = echo_plugin_stop_processing;
  p->plugin.reset = echo_plugin_reset;
  p->plugin.process = echo_plugin_process;
  p->plugin.get_extension = echo_plugin_get_extension;
  p->plugin.on_main_thread = echo_plugin_on_main_thread;

  // Don't call into the host here

  return &p->plugin;
}

/////////////////////////
// clap_plugin_factory //
/////////////////////////

static struct {
   const clap_plugin_descriptor_t *desc;
   clap_plugin_t *(CLAP_ABI *create)(const clap_host_t *host);
} s_plugins[] = {
  {
    .desc = &s_echo_plugin_desc,
    .create = echo_plugin_create,
  },
};

static uint32_t plugin_factory_get_plugin_count(const struct clap_plugin_factory *factory) {
  return sizeof(s_plugins) / sizeof(s_plugins[0]);
}

static const clap_plugin_descriptor_t *
plugin_factory_get_plugin_descriptor(const struct clap_plugin_factory *factory, uint32_t index) {
  return s_plugins[index].desc;
}

static const clap_plugin_t *plugin_factory_create_plugin(const struct clap_plugin_factory *factory,
                                                         const clap_host_t                *host,
                                                         const char *plugin_id) {
  if (!clap_version_is_compatible(host->clap_version)) {
    return NULL;
  }

  const int N = sizeof(s_plugins) / sizeof(s_plugins[0]);
  for (int i = 0; i < N; ++i)
    if (!strcmp(plugin_id, s_plugins[i].desc->id))
      return s_plugins[i].create(host);

  return NULL;
}

static const clap_plugin_factory_t s_plugin_factory = {
  .get_plugin_count = plugin_factory_get_plugin_count,
  .get_plugin_descriptor = plugin_factory_get_plugin_descriptor,
  .create_plugin = plugin_factory_create_plugin,
};

////////////////
// clap_entry //
////////////////

static bool entry_init(const char *plugin_path) {
  // perform the plugin initialization
  return true;
}

static void entry_deinit(void) {
  // perform the plugin de-initialization
}

#ifdef CLAP_HAS_THREAD
static mtx_t g_entry_lock;
static once_flag g_entry_once = ONCE_FLAG_INIT;
#endif

static int g_entry_init_counter = 0;

#ifdef CLAP_HAS_THREAD
// Initializes the necessary mutex for the entry guard
static void entry_init_guard_init(void) {
  mtx_init(&g_entry_lock, mtx_plain);
}
#endif

// Thread safe init counter
static bool entry_init_guard(const char *plugin_path) {
#ifdef CLAP_HAS_THREAD
  call_once(&g_entry_once, entry_init_guard_init);

  mtx_lock(&g_entry_lock);
#endif

  const int cnt = ++g_entry_init_counter;
  assert(cnt > 0);

  bool succeed = true;
  if (cnt == 1) {
    succeed = entry_init(plugin_path);
    if (!succeed)
      g_entry_init_counter = 0;
  }

#ifdef CLAP_HAS_THREAD
  mtx_unlock(&g_entry_lock);
#endif

  return succeed;
}

// Thread safe deinit counter
static void entry_deinit_guard(void) {
#ifdef CLAP_HAS_THREAD
  call_once(&g_entry_once, entry_init_guard_init);

  mtx_lock(&g_entry_lock);
#endif

  const int cnt = --g_entry_init_counter;
  assert(cnt >= 0);

  bool succeed = true;
  if (cnt == 0)
    entry_deinit();

#ifdef CLAP_HAS_THREAD
  mtx_unlock(&g_entry_lock);
#endif
}

static const void *entry_get_factory(const char *factory_id) {
#ifdef CLAP_HAS_THREAD
  call_once(&g_entry_once, entry_init_guard_init);
#endif

  assert(g_entry_init_counter > 0);
  if (g_entry_init_counter <= 0)
    return NULL;

  if (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID))
    return &s_plugin_factory;
  return NULL;
}

// This symbol will be resolved by the host
CLAP_EXPORT const clap_plugin_entry_t clap_entry = {
  .clap_version = CLAP_VERSION_INIT,
  .init = entry_init_guard,
  .deinit = entry_deinit_guard,
  .get_factory = entry_get_factory,
};
