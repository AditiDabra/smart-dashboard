This is a [Next.js](https://nextjs.org) project bootstrapped with [`create-next-app`](https://nextjs.org/docs/app/api-reference/cli/create-next-app).

## Getting Started

First, to run the development server:

```bash
npm run dev
# or
yarn dev
# or
pnpm dev
# or
bun dev
```

Open [http://localhost:3000](http://localhost:3000) with your browser to see the result.

You can start editing the page by modifying `app/page.tsx`. The page auto-updates as you edit the file.

This project uses [`next/font`](https://nextjs.org/docs/app/building-your-application/optimizing/fonts) to automatically optimize and load [Geist](https://vercel.com/font), a new font family for Vercel.

## Learn More

To learn more about Next.js, take a look at the following resources:

- [Next.js Documentation](https://nextjs.org/docs) - learn about Next.js features and API.
- [Learn Next.js](https://nextjs.org/learn) - an interactive Next.js tutorial.

You can check out [the Next.js GitHub repository](https://github.com/vercel/next.js) - your feedback and contributions are welcome!

## Deploy on Vercel

The easiest way to deploy your Next.js app is to use the [Vercel Platform](https://vercel.com/new?utm_medium=default-template&filter=next.js&utm_source=create-next-app&utm_campaign=create-next-app-readme) from the creators of Next.js.

Check out our [Next.js deployment documentation](https://nextjs.org/docs/app/building-your-application/deploying) for more details.


# Quick Start Guide

## What's New?

### 1️⃣ Smart Alert System

- **Alerts only trigger** when a sustained problem exists (>50% of readings exceed threshold)
- **Prevents spam** - won't create duplicate alerts while one is active
- **Auto-resolves** when readings normalize (>70% back to normal)
- **Tracks duration** - know how long each alert was active
- **3 severity levels** - Low, Medium, High based on how much reading exceeds threshold

### 2️⃣ New API Endpoint

```
GET /api/alerts?status=active&severity=high&limit=50
```

Filter alerts by:

- Status (active/resolved)
- Severity (low/medium/high)
- Machine ID
- Get pagination support

### 3️⃣ Enhanced Dashboard

- Real-time stats with icons
- Active alert banner for current machine
- Recent alerts sidebar
- Connection status indicator
- Dark mode support

### 4️⃣ Improved Alerts Page

- Advanced filtering (status, severity, machine)
- Alert duration display (for resolved alerts)
- Real-time refresh
- Better visual hierarchy
- Dark mode support

## Key Metrics Displayed

**On Dashboard:**

- 🔴 Active Alerts (total)
- ⚠️ High Severity Count
- ⚡ Live Current (Amps)
- 📊 Live Voltage (Volts)

**On Alerts Page:**

- ✅ Active vs Resolved counts
- Alert history with filters
- Duration of each resolved alert
- Real-time updates

## Alert Thresholds

- **Current**: 200A (default)
- **Voltage**: 40V (default)
- **Breach needed for alert**: >50% of last 100 readings above threshold
- **Recovery for resolution**: >70% of last 100 readings below threshold

## Default Machine Settings

```
maxCurrent: 200A
maxVoltage: 40V
```

To change: Update the machine settings in database or settings page (coming soon)

## Status Indicators

### In Dashboard

🟢 **Connected** - Receiving live data
🟡 **Waiting...** - No data yet
🔴 **Active Alert** - Pulsing animation

### In Alerts Table

🔴 **Active** - Alert is ongoing
✅ **Resolved** - Alert finished with duration shown

## Severity Colors

🔴 **HIGH** - Red (Excess > 15)
🟡 **MEDIUM** - Yellow (Excess 5-15)
🔵 **LOW** - Blue (Excess ≤ 5)

## Test the System

### Send sample data:

```bash
# Normal reading
curl -X POST http://localhost:3001/api/readings \
  -H "Content-Type: application/json" \
  -d '{"machineId": 1, "voltage": 42, "current": 145}'

# High reading (might trigger alert after 50+ readings)
curl -X POST http://localhost:3001/api/readings \
  -H "Content-Type: application/json" \
  -d '{"machineId": 1, "voltage": 42, "current": 165}'
```

### Check alerts:

```bash
# Get all active alerts
curl http://localhost:3001/api/alerts?status=active

# Get high severity alerts
curl http://localhost:3001/api/alerts?severity=high

# Get alerts for machine 1
curl http://localhost:3001/api/alerts?machineId=1
```

## Troubleshooting

| Issue                   | Solution                                               |
| ----------------------- | ------------------------------------------------------ |
| No alerts showing       | Wait for 100+ readings to be sent                      |
| Alerts won't resolve    | Readings must be <threshold for 70%+ of window         |
| /api/alerts returns 404 | Backend server not running on port 3001                |
| No real-time updates    | Check Socket.IO connection (browser F12 console)       |
| Duration shows as "--"  | Alert still active (duration only shows when resolved) |

## Files Modified/Created

✅ **Backend:**

- `server.ts` - New alert logic and API endpoint
- `prisma/schema.prisma` - New models and fields
- `prisma/migrations/20251126_add_threshold_breach_duration/` - Database migration

✅ **Frontend:**

- `src/app/dashboard/page.tsx` - Enhanced dashboard with new cards and alert banner
- `src/app/alerts/page.tsx` - Complete rewrite with filters and real-time updates
- `src/components/AlertTable.tsx` - Enhanced table with duration and status columns
- `src/lib/api.ts` - New types and fetchAlerts function

## Next Steps (Optional)

1. Add machine settings page to configure thresholds
2. Add alert acknowledgment feature
3. Add email/SMS notifications
4. Add alert history export
5. Add prediction/forecasting alerts
6. Add alert suppression rules

---

**Everything is ready to use! 🚀**

