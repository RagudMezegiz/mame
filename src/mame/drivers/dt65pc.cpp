// license:BSD-3-Clause
// copyright-holders:David Terhune
/****************************************************************************

	DT65PC
	Dave Terhune's 65xx Personal Computer

	This is a test bench for emulating the DT65PC, a computer that may never
	actually be constructed for real.

	Main CPU: 65C816

****************************************************************************/

#include "emu.h"
#include "cpu/g65816/g65816.h"
#include "video/v9938.h"

class dt65pc_state : public driver_device
{
public:
	dt65pc_state(const machine_config& config, device_type type, const char *tag) :
		driver_device(config, type, tag),
		m_maincpu(*this, "maincpu"),
		m_screen(*this, "screen"),
		m_video(*this, "video")
	{
	}

	void dt65pc(machine_config& config);

	void dt65pc_mem(address_map& map);

private:
	required_device<g65816_device> m_maincpu;
	required_device<screen_device> m_screen;
	required_device<v9938_device> m_video;
};

void dt65pc_state::dt65pc(machine_config& config)
{
	G65816(config, m_maincpu, 21.477272_MHz_XTAL / 6);
	m_maincpu->set_addrmap(AS_PROGRAM, &dt65pc_state::dt65pc_mem);

	SCREEN(config, m_screen, SCREEN_TYPE_RASTER);
	m_screen->set_refresh_hz(60);
	m_screen->set_size(640, 480);

	V9938(config, m_video, 21.477272_MHz_XTAL);
	m_video->set_screen_ntsc(m_screen);
	m_video->set_vram_size(0x10000);
}

void dt65pc_state::dt65pc_mem(address_map& map)
{
	map(0x0000, 0xcfff).ram();    // low RAM
	map(0xd000, 0xd003).rw(m_video, FUNC(v9938_device::read), FUNC(v9938_device::write));
	map(0xd004, 0xdfff).noprw();  // future IO space
	map(0xe000, 0xffff).rom();    // kernel ROM
	map(0x10000, 0xffffff).ram(); // high RAM
}

static INPUT_PORTS_START( dt65pc )
INPUT_PORTS_END

ROM_START( dt65pc )
	ROM_REGION( 0x2000, "maincpu", 0 )
	//ROM_LOAD( "dt65pc.rom", 0xe0000, 0x2000, 0 ) // Add CRC and SHA1 with final kernel
ROM_END

//    YEAR,   NAME, PARENT, COMPAT, MACHINE,  INPUT,        CLASS,       INIT,    COMPANY, FULLNAME, FLAGS
COMP( 2019, dt65pc,      0,      0,  dt65pc, dt65pc, dt65pc_state, empty_init, "dterhune", "Dave Terhune's 65xx PC", MACHINE_NO_SOUND_HW | MACHINE_IS_INCOMPLETE)
