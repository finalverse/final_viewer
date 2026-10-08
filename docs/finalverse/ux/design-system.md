# Finalverse experience design system

Phase 4A wraps LLUI rather than replacing it. The supplied Harbor/Lumi moodboard informs warmth, quiet navy surfaces, generous spacing, and a world-first composition. It is a direction image, not a promise that the current primitive Harbor scene has those assets or that Lumi has a photoreal avatar.

| Semantic role | LLUI token | Value (sRGB) |
|---|---|---|
| Background | FinalverseBackground | #09111B; value `0.035 0.065 0.105 1` |
| Panel | FinalversePanel | `0.065 0.105 0.155 0.97` |
| Control surface | FinalverseSurface | `0.12 0.18 0.24 1` |
| Main text | FinalverseText | `0.98 0.96 0.92 1` |
| Secondary text | FinalverseSecondary | `0.72 0.79 0.83 1` |
| Primary action | FinalverseAccent | `0.96 0.77 0.48 1` |
| AI/citizen | FinalverseAI | `0.58 0.85 0.77 1` |
| Success / warning / danger | FinalverseSuccess / Warning / Danger | Named in `skins/default/colors.xml` |
| Selection | FinalverseSelection | `0.55 0.78 0.95 1` |

New tokens apply to the facade only. Inherited advanced panels retain their own themes. Gold primary buttons use dark text; secondary buttons use warm white. Status is written in words, not conveyed by color alone. Disabled approval controls distinguish an unreviewed request from an actionable preview.

Typography uses the already bundled Inter variable font through `SansSerif`, `SansSerifMedium`, `SansSerifLarge`, and `SansSerifSmall`. No font download is required. Headings are Large; body and controls are SansSerif; location/technical controls are Small. LLUI's existing scale preference scales the entire surface. New text wraps rather than depending on a wide desktop sidebar.

Spacing: 8 px internal cadence, 16 px HUD inset, 20 px floater margin, 36 px main controls and 30–32 px secondary controls, all in logical viewer pixels. New floaters are 432 × 442 logical px. The AI preview is a 120 px scrollable text area, preserving room for approval and Undo at enlarged scale. A desktop viewport of at least 800 × 450 logical px is required; smaller/mobile layouts need a separate responsive implementation. Existing `Rounded_Rect` nine-slice texture supplies approximately 6 px corners; there is no shader blur, backdrop capture, animated glow, or continuous inference. Surfaces use near-opaque navy for readability over bright or dark terrain. Existing focus borders and tab navigation remain enabled.

The existing licensed Finalverse Lumi portrait is reused by texture alias `first_login_image`. No provider asset library or new raster artwork has been downloaded. Upstream copyright notices remain intact.

Contrast calculations on opaque tokens: main text/control surface 12.74:1, secondary text/control surface 8.17:1, AI text/panel 10.73:1, and dark text/gold 11.85:1. The panel is 97% opaque; these token calculations are not a full accessibility certification. Scale and focus are verified separately in the validation report.
